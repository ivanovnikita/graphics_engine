#pragma once

#include "swapchain.h"
#include "depth_buffer.h"

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

namespace ge
{
    vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device&,
        const vk::RenderPass&,
        const vk::ImageView& surface_image,
        const vk::ImageView& depth_buffer,
        const vk::Extent2D&
    );

    std::vector<vk::UniqueFramebuffer> create_framebuffers
    (
        const vk::Device&,
        const vk::RenderPass&,
        const SwapchainData&,
        const DepthBuffer&
    );
}
