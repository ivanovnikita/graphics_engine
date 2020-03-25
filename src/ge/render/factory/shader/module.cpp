#include "ge/render/factory/shader/module.h"

namespace ge::factory
{
    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device& logical_device
      , const std::span<const uint32_t> spirv_code
    )
    {
        const vk::ShaderModuleCreateInfo creation_info
        {
            vk::ShaderModuleCreateFlags{}
          , spirv_code.size() * sizeof(uint32_t)
          , spirv_code.data()
        };

        return logical_device.createShaderModuleUnique(creation_info);
    }
}
