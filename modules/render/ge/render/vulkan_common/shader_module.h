#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device&,
        std::span<const uint32_t> spirv_code
    );
}
