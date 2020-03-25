#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::factory
{
    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device& logical_device
      , const std::span<const uint32_t> spirv_code
    );
}

