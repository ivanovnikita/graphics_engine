#pragma once

#include "ge/render/factory/command_pool.h"
#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::factory
{
    std::vector<vk::CommandBuffer> create_command_buffer
    (
        const vk::Device&
        , const vk::CommandPool&
        , const std::span<const vk::UniqueFramebuffer>&
        , const vk::RenderPass&
        , const vk::Extent2D&
        , const vk::ClearColorValue& background_color
        , const vk::Pipeline&
        , const vk::PipelineLayout& pipeline_layout
        , const std::span<const vk::DescriptorSet> descriptor_sets
        , const vk::Buffer& vertices
        , const vk::Buffer& colors
        , const vk::Buffer& indices
        , const size_t indices_count
    );
}
