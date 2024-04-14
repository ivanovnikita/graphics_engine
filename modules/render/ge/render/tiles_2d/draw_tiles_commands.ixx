module;

#include <vulkan/vulkan.hpp>

#include <span>

export module tiles_2d.draw_tiles_commands;

export import tiles_2d.polygons_in_device_memory;

namespace ge::tiles
{
    export std::vector<vk::UniqueCommandBuffer> draw_tiles_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& triangles_pipeline,
        const vk::Pipeline& lines_pipeline,
        const vk::PipelineLayout&,
        const std::span<const vk::UniqueDescriptorSet>,
        const PolygonsInDeviceMemory&
    );
}
