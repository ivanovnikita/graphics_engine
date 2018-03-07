#include "shader.h"

#include "shaderc/shaderc.hpp"

namespace ge::impl::factory::shader
{

    vk::UniqueShaderModule create()
    {
        return vk::UniqueShaderModule();
    }

}
