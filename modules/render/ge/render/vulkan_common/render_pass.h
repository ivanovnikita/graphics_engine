#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueRenderPass create_render_pass_default
    (
        const vk::Device&,
        const vk::Format& present_format,
        const vk::Format& depth_format
    );
}
