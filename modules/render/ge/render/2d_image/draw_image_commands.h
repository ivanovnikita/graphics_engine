#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::image
{
    std::vector<vk::UniqueCommandBuffer> draw_image_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline&,
        const vk::PipelineLayout&,
        const std::span<const vk::UniqueDescriptorSet>
    );
}
