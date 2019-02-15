#pragma once

#include "ge/render/utils/span.hpp"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device& logical_device
      , const Span<const uint32_t> spirv_code
    );
}

