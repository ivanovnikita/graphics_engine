#include "shader.h"
#include "exception.h"

#include <shaderc/shaderc.hpp>

#include <fstream>

namespace ge::impl::factory::shader
{

    namespace
    {
        shaderc_shader_kind shader_kind(const fs::path& shader_code)
        {
            const auto extension = shader_code.extension().u8string();
            if (extension == "vert")
            {
                return shaderc_shader_kind::shaderc_vertex_shader;
            }
            else if (extension == "frag")
            {
                return shaderc_shader_kind::shaderc_fragment_shader;
            }

            GE_THROW(invalid_shader, "Unknown shader type");
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
                    GE_THROW(invalid_shader, "File is empty");
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
                GE_THROW(invalid_shader, "Invalid file");
            }
        }
    }

    vk::UniqueShaderModule create
    (
        const vk::Device& logical_device
      , const fs::path& shader_code_file
    )
    {
        shaderc::Compiler compiler;

        const std::string& code = read_file_content(shader_code_file);
        const auto& compilation_result = compiler.CompileGlslToSpv
        (
            code
          , shader_kind(shader_code_file)
          , shader_code_file.filename().c_str()
        );

        const std::vector<uint32_t> spirv_code{compilation_result.cbegin(), compilation_result.cend()};
        const vk::ShaderModuleCreateInfo creation_info
        {
            vk::ShaderModuleCreateFlags{}
          , spirv_code.size()
          , spirv_code.data()
        };

        return logical_device.createShaderModuleUnique(creation_info);
    }

}
