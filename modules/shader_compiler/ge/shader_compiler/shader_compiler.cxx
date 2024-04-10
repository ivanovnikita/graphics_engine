module;

#include <vulkan/vulkan.hpp>

#include <shaderc/shaderc.hpp>

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

module shader_compiler;

namespace ge
{
    namespace
    {
        vk::ShaderStageFlagBits shader_kind(const fs::path& shader_code)
        {
            const auto extension = shader_code.extension().string();
            if (extension == ".vert")
            {
                return vk::ShaderStageFlagBits::eVertex;
            }
            else if (extension == ".frag")
            {
                return vk::ShaderStageFlagBits::eFragment;
            }

            throw std::logic_error("Unknown shader type");
        }

        shaderc_shader_kind to_shaderc_shader_kind(const vk::ShaderStageFlagBits vk_shader_kind)
        {
            switch (vk_shader_kind)
            {
            case vk::ShaderStageFlagBits::eVertex: return shaderc_shader_kind::shaderc_vertex_shader;
            case vk::ShaderStageFlagBits::eFragment: return shaderc_shader_kind::shaderc_fragment_shader;
            case vk::ShaderStageFlagBits::eCompute: return shaderc_shader_kind::shaderc_compute_shader;
            case vk::ShaderStageFlagBits::eGeometry: return shaderc_shader_kind::shaderc_geometry_shader;
            case vk::ShaderStageFlagBits::eTessellationControl: return shaderc_shader_kind::shaderc_tess_control_shader;
            case vk::ShaderStageFlagBits::eTessellationEvaluation: return shaderc_shader_kind::shaderc_tess_evaluation_shader;

            case vk::ShaderStageFlagBits::eAll:
            case vk::ShaderStageFlagBits::eAllGraphics:
            case vk::ShaderStageFlagBits::eRaygenNV:
            case vk::ShaderStageFlagBits::eAnyHitNV:
            case vk::ShaderStageFlagBits::eClosestHitNV:
            case vk::ShaderStageFlagBits::eMissNV:
            case vk::ShaderStageFlagBits::eIntersectionNV:
            case vk::ShaderStageFlagBits::eCallableNV:
            case vk::ShaderStageFlagBits::eTaskNV:
            case vk::ShaderStageFlagBits::eMeshNV:
            case vk::ShaderStageFlagBits::eSubpassShadingHUAWEI:
            {
                throw std::logic_error("Unknown shader type");
            }
            }

            throw std::logic_error("Unknown shader type");
        }

        std::string read_file_content(const fs::path& filename)
        {
            try
            {
                std::ifstream file;
                file.exceptions(std::ifstream::failbit);
                file.open(filename, std::ios::binary | std::ios::ate);

                const auto fileSize = file.tellg();
                if (fileSize < 1)
                {
                    throw std::runtime_error("File is empty");
                }

                std::string buffer;
                buffer.resize(static_cast<uint32_t>(fileSize));

                file.seekg(0);
                file.read(buffer.data(), fileSize);

                file.close();

                return buffer;
            }
            catch (const std::fstream::failure&)
            {
                throw std::runtime_error("Invalid file");
            }
        }

        std::string filename(fs::path path)
        {
            return path.replace_extension().filename().string();
        }

        using ShaderName = std::string;
        using CompiledShader = std::vector<uint32_t>;

        GeneratedCppSources generate_cpp_sources
        (
            const std::map<ShaderName, std::map<vk::ShaderStageFlagBits, CompiledShader>>& compiled_shaders
            , const std::string_view target_namespace
            , const std::string_view target_filename
        )
        {
            std::ostringstream interface;
            std::ostringstream implementation;

            {
                std::ostringstream common;
                common <<
                    "namespace " << target_namespace << "\n"
                    "{\n";

                interface
                    << "module;\n\n"
                       "#include <span>\n\n"
                       "#include <cstdint>\n\n"
                    <<
                       "export module " << target_filename << ";\n\n"
                    <<
                       "namespace vk\n"
                       "{\n"
                       "    export enum class ShaderStageFlagBits : uint32_t;\n"
                       "}\n\n"
                    << common.str();
                implementation
                    <<
                       "module;\n\n"
                       "#include <vulkan/vulkan.hpp>\n\n"
                       "#include <array>\n\n"
                       "#include <span>\n\n"
                       "#include <cstdint>\n\n"
                    << "module " << target_filename << ";\n\n"
                    << common.str();
            }

            const auto create_shader_name = []
            (
                const ShaderName& shader_name
                , const vk::ShaderStageFlagBits& shader_kind
            ) -> std::string
            {
                return shader_name + "_" + to_string(shader_kind);
            };

            {
                interface << "    export enum class ShaderName\n"
                          "    {\n";

                std::string comma{""};
                for (const auto& [shader_name, shaders] : compiled_shaders)
                {
                    for (const auto& [shader_kind, shader] : shaders)
                    {
                        if (shader.empty())
                        {
                            continue;
                        }

                        interface << "        " << comma << create_shader_name(shader_name, shader_kind) << "\n";

                        comma = ", ";
                    }
                }

                interface
                    << "    };\n\n"
                    << "    export std::span<const uint32_t> get_shader(ShaderName);\n"
                    << "    export vk::ShaderStageFlagBits get_shader_kind(ShaderName);\n";
                interface << "}\n";
            }

            for (const auto& [shader_name, shaders] : compiled_shaders)
            {
                for (const auto& [shader_kind, shader] : shaders)
                {
                    if (shader.empty())
                    {
                        continue;
                    }

                    const auto start = std::chrono::high_resolution_clock::now();


                    implementation <<
                        "    static constexpr std::array<uint32_t, "
                        << shader.size()
                        << "> "
                        << create_shader_name(shader_name, shader_kind)
                        << "\n" << "    {\n";

                    implementation << "        " << shader.front();
                    for (size_t i = 1; i < shader.size(); ++i)
                    {
                        interface << ", " << shader[i] << "";

                        if (i % 10 == 0)
                        {
                            interface << "\n      ";
                        }
                    }
                    implementation << "\n    };\n\n";

                    const auto stop = std::chrono::high_resolution_clock::now();

                    std::cout
                        << "Generating cpp sources for shader: " << shader_name
                        << " [" << to_string(shader_kind) << "] ["
                        << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                        << " millisec]"
                        << std::endl;
                }
            }

            {
                implementation << "    std::span<const uint32_t> get_shader(const ShaderName shader_name)\n"
                          "    {\n";

                implementation << "        switch(shader_name)\n"
                          "        {\n";

                for (const auto& [shader_name, shaders] : compiled_shaders)
                {
                    for (const auto& [shader_kind, shader] : shaders)
                    {
                        if (shader.empty())
                        {
                            continue;
                        }

                        const std::string name = create_shader_name(shader_name, shader_kind);
                        implementation
                            << "        case ShaderName::"
                            << name
                            << ": return "
                            << name
                            << ";\n";
                    }
                }

                implementation
                    << "        }\n"
                       "        __builtin_unreachable();\n";

                implementation << "    }\n\n";
            }

            {
                implementation << "    vk::ShaderStageFlagBits get_shader_kind(const ShaderName shader_name)\n"
                          "    {\n";

                implementation << "        switch(shader_name)\n"
                          "        {\n";

                for (const auto& [shader_name, shaders] : compiled_shaders)
                {
                    for (const auto& [shader_kind, shader] : shaders)
                    {
                        if (shader.empty())
                        {
                            continue;
                        }

                        const std::string name = create_shader_name(shader_name, shader_kind);
                        implementation
                            << "        case ShaderName::"
                            << name
                            << ": return "
                            << "vk::ShaderStageFlagBits::e"
                            << to_string(shader_kind)
                            << ";\n";
                    }
                }

                implementation
                    << "        }\n"
                       "        __builtin_unreachable();\n";

                implementation << "    }\n\n";
            }

            implementation << "}\n";

            GeneratedCppSources result;
            result.interface = interface.str();
            result.implementation = implementation.str();

            return result;
        }
    }

    GeneratedCppSources compile_shaders_into_cpp_sources
    (
        const fs::path& shaders_dir
        , const std::string_view target_namespace
        , const std::string_view target_filename
    )
    {
        std::map<ShaderName, std::map<vk::ShaderStageFlagBits, CompiledShader>> compiled_shaders;

        shaderc::CompileOptions options;
//        options.SetInvertY(true); // TODO: it seems to be broken
        options.SetWarningsAsErrors();

        shaderc::Compiler compiler;
        for (const auto& shader : fs::directory_iterator(shaders_dir))
        {
            const auto start = std::chrono::high_resolution_clock::now();

            const vk::ShaderStageFlagBits vk_shader_kind = shader_kind(shader);
            const std::string& code = read_file_content(shader);
            const auto& compilation_result = compiler.CompileGlslToSpv
            (
                code
              , to_shaderc_shader_kind(vk_shader_kind)
              , shader.path().filename().c_str()
              , options
            );

            if (compilation_result.GetCompilationStatus() != shaderc_compilation_status_success)
            {
                throw std::runtime_error(compilation_result.GetErrorMessage());
            }

            CompiledShader spirv_code{compilation_result.cbegin(), compilation_result.cend()};

            compiled_shaders[filename(shader)].emplace
            (
                vk_shader_kind
                , std::move(spirv_code)
            );

            const auto stop = std::chrono::high_resolution_clock::now();

            std::cout
                << "Compiling shader: "
                << shader.path().filename()
                << " ["
                << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
                << " millisec]"
                << std::endl;
        }

        return generate_cpp_sources(compiled_shaders, target_namespace, target_filename);
    }

    void save_sources
    (
        const GeneratedCppSources& sources
        , const fs::path& target_dir
        , const std::string_view target_filename
    )
    {
        {
            std::ofstream file;
            file.exceptions(std::ifstream::failbit);
            file.open((target_dir / target_filename).string() + ".ixx", std::ios::out);

            file << sources.interface;
            file.flush();

            file.close();
        }
        {
            std::ofstream file;
            file.exceptions(std::ifstream::failbit);
            file.open((target_dir / target_filename).string() + ".cxx", std::ios::out);

            file << sources.implementation;
            file.flush();

            file.close();
        }
    }
}
