#include "shader_compiler.h"

#include <vulkan/vulkan.hpp>

#include <shaderc/shaderc.hpp>

#include <map>
#include <fstream>
#include <sstream>

namespace ge
{
    namespace
    {
        vk::ShaderStageFlagBits shader_kind(const fs::path& shader_code)
        {
            const auto extension = shader_code.extension().u8string();
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
                    "#include <array>\n"
                    "\n"
                    "namespace " << target_namespace << "\n"
                    "{\n";

                header << "#pragma once\n\n" << common.str();
                source
                    << R"(#include ")" << target_filename << R"(.h")"
                    << "\n\n"
                    << common.str();
            }

            for (const auto& [shader_name, shaders] : compiled_shaders)
            {
                for (const auto& [shader_kind, shader] : shaders)
                {
                    if (shader.empty())
                    {
                        continue;
                    }

                    std::ostringstream common;

                    common <<
                        "    extern const std::array<uint32_t, "
                        << shader.size()
                        << "> "
                        << shader_name
                        << "_"
                        << to_string(shader_kind);

                    header << common.str() << ";\n";

                    source << common.str() << "\n" << "    {\n";
                    source << "         " << shader.front() << "\n";
                    for (size_t i = 1; i < shader.size(); ++i)
                    {
                        source << "        , " << shader[i] << "\n";
                    }
                    source << "    };\n";
                }
            }
            header << "}";
            source << "}";;

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

        shaderc::Compiler compiler;
        for (const auto& shader : fs::directory_iterator(shaders_dir))
        {
            const vk::ShaderStageFlagBits vk_shader_kind = shader_kind(shader);
            const std::string& code = read_file_content(shader);
            const auto& compilation_result = compiler.CompileGlslToSpv
            (
                code
              , to_shaderc_shader_kind(vk_shader_kind)
              , shader.path().filename().c_str()
            );
            CompiledShader spirv_code{compilation_result.cbegin(), compilation_result.cend()};

            compiled_shaders[filename(shader)].emplace
            (
                vk_shader_kind
                , std::move(spirv_code)
            );
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

    return 0;
}
