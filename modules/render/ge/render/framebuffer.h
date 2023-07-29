#pragma once

#include "ge/render/swapchain.h"

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

namespace ge
{
    vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device&,
        const vk::RenderPass&,
        const vk::ImageView&,
        const vk::Extent2D&
    );

    std::vector<vk::UniqueFramebuffer> create_framebuffers
    (
        const vk::Device&,
        const vk::RenderPass&,
        const SwapchainData&
    );
}
