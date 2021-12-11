#include "shader_compiler.h"

#include <vulkan/vulkan.hpp>

#include <shaderc/shaderc.hpp>

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

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
            std::ostringstream header;
            std::ostringstream source;

            {
                std::ostringstream common;
                common <<
                    "namespace " << target_namespace << "\n"
                    "{\n";

                header
                    << "#pragma once\n\n"
                       "#include <span>\n\n"
                       "#include <cstdint>\n\n"
                       "namespace vk\n"
                       "{\n"
                       "    enum class ShaderStageFlagBits : uint32_t;\n"
                       "}\n\n"
                    << common.str();
                source
                    << R"(#include ")" << target_filename << R"(.h")"
                    << "\n\n"
                       "#include <vulkan/vulkan.hpp>\n\n"
                       "#include <array>\n\n"
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
                header << "    enum class ShaderName\n"
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

                        header << "        " << comma << create_shader_name(shader_name, shader_kind) << "\n";

                        comma = ", ";
                    }
                }

                header
                    << "    };\n\n"
                    << "    std::span<const uint32_t> get_shader(ShaderName);\n"
                    << "    vk::ShaderStageFlagBits get_shader_kind(ShaderName);\n";
                header << "}\n";
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


                    source <<
                        "    static constexpr std::array<uint32_t, "
                        << shader.size()
                        << "> "
                        << create_shader_name(shader_name, shader_kind)
                        << "\n" << "    {\n";

                    source << "        " << shader.front();
                    for (size_t i = 1; i < shader.size(); ++i)
                    {
                        source << ", " << shader[i] << "";

                        if (i % 10 == 0)
                        {
                            source << "\n      ";
                        }
                    }
                    source << "\n    };\n\n";

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
                source << "    std::span<const uint32_t> get_shader(const ShaderName shader_name)\n"
                          "    {\n";

                source << "        switch(shader_name)\n"
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
                        source
                            << "        case ShaderName::"
                            << name
                            << ": return "
                            << name
                            << ";\n";
                    }
                }

                source
                    << "        }\n"
                       "        __builtin_unreachable();\n";

                source << "    }\n\n";
            }

            {
                source << "    vk::ShaderStageFlagBits get_shader_kind(const ShaderName shader_name)\n"
                          "    {\n";

                source << "        switch(shader_name)\n"
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
                        source
                            << "        case ShaderName::"
                            << name
                            << ": return "
                            << "vk::ShaderStageFlagBits::e"
                            << to_string(shader_kind)
                            << ";\n";
                    }
                }

                source
                    << "        }\n"
                       "        __builtin_unreachable();\n";

                source << "    }\n\n";
            }

            source << "}\n";

            GeneratedCppSources result;
            result.header = header.str();
            result.source = source.str();

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
            file.open((target_dir / target_filename).string() + ".h", std::ios::out);

            file << sources.header;
            file.flush();

            file.close();
        }
        {
            std::ofstream file;
            file.exceptions(std::ifstream::failbit);
            file.open((target_dir / target_filename).string() + ".cpp", std::ios::out);

            file << sources.source;
            file.flush();

            file.close();
        }
    }
}

int main(int argc, char* argv[])
{
    using namespace ge;

    try
    {
        if (argc < 5)
        {
            throw std::logic_error("Invalid input options");
        }

        const fs::path shaders_dir{argv[1]};
        const std::string target_namespace{argv[2]};
        const fs::path target_dir{argv[3]};
        const std::string target_name{argv[4]};

        const GeneratedCppSources generated = compile_shaders_into_cpp_sources
        (
            shaders_dir
            , target_namespace
            , target_name
        );

        if (not fs::exists(target_dir))
        {
            fs::create_directory(target_dir);
        }

        save_sources(generated, target_dir, target_name);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected compilation error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
