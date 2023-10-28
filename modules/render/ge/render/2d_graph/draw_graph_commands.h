#pragma once

#include "graph_in_device_memory.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::graph
{
    std::vector<vk::UniqueCommandBuffer> draw_graph_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& arcs_pipeline,
        const vk::Pipeline& vertices_pipeline,
        const vk::PipelineLayout&,
        const std::span<const vk::UniqueDescriptorSet>,
        const GraphInDeviceMemory&
    );
}
