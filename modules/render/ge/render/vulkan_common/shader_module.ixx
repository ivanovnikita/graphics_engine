module;

#include "vulkan_fwds.h"

#include <span>

export module vulkan_common.shader_module;

namespace ge
{
    export vk::UniqueShaderModule create_shader_module
    (
        const vk::Device&,
        std::span<const uint32_t> spirv_code
    );
}
