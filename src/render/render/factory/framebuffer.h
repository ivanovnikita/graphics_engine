#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device&
      , const vk::RenderPass&
      , const vk::ImageView&
      , const vk::Extent2D&
    );
}
