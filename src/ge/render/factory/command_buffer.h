#pragma once

#include "ge/render/factory/command_pool.h"
#include "ge/render/factory/buffer.hpp"
#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::factory
{
    std::vector<vk::CommandBuffer> draw_graph_commands
    (
        const vk::Device&
        , const vk::CommandPool&
        , const std::span<const vk::UniqueFramebuffer>&
        , const vk::RenderPass&
        , const vk::Extent2D&
        , const vk::ClearColorValue& background_color
        , const vk::Pipeline& arcs_pipeline
        , const vk::Pipeline& vertices_pipeline
        , const vk::PipelineLayout& pipeline_layout
        , const std::span<const vk::DescriptorSet> descriptor_sets
        , const GraphInDeviceMemory&
    );

    std::vector<vk::CommandBuffer> draw_polygons_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& polygons_pipeline,
        const vk::PipelineLayout& pipeline_layout,
        const std::span<const vk::DescriptorSet> descriptor_sets,
        const PolygonsInDeviceMemory&
    );
}
