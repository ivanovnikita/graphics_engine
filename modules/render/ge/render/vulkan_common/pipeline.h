#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::UniquePipeline create_pipeline_default
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
        const bool depth_buffer_enabled
    );
}
