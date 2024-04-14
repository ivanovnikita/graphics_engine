module;

#include <vulkan/vulkan.hpp>

#include <span>

export module vulkan_common.pipeline;

export import antialiasing;

namespace ge
{
    export vk::UniquePipeline create_pipeline_default
    (
        const vk::Device&,
        const std::span<const vk::PipelineShaderStageCreateInfo>,
        const std::span<const vk::VertexInputBindingDescription>,
        const std::span<const vk::VertexInputAttributeDescription>,
        const vk::PipelineInputAssemblyStateCreateInfo,
        const vk::PipelineRasterizationStateCreateInfo,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::PipelineLayout&,
        const bool depth_buffer_enabled,
        const Antialiasing&
    );
}
