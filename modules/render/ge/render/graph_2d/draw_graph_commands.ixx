module;

#include <vulkan/vulkan.hpp>

#include <span>

export module graph_2d.draw_graph_commands;

export import graph_2d.graph_in_device_memory;

namespace ge::graph
{
    export std::vector<vk::UniqueCommandBuffer> draw_graph_commands
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
