#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::framebuffer
{

    vk::UniqueFramebuffer create
    (
        const vk::Device&
      , const vk::RenderPass&
      , const vk::ImageView&
      , const vk::Extent2D&
    );

}
