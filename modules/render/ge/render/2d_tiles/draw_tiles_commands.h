#pragma once

#include "polygons_in_device_memory.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::tiles
{
    std::vector<vk::UniqueCommandBuffer> draw_tiles_commands
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
