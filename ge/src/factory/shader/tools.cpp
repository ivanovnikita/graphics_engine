#include "tools.h"

#include "exception.h"

namespace ge::impl::factory::shader::tools
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

        GE_THROW(invalid_shader, "Unknown shader type");
    }

}
