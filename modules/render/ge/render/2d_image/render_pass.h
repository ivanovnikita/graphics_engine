#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image
{
    vk::UniqueRenderPass create_render_pass
    (
        const vk::Device&,
        const vk::Format&
    );
}
