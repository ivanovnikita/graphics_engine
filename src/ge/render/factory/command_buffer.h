#pragma once

#include "ge/render/factory/command_pool.h"
#include "ge/render/utils/span.hpp"
#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::vector<vk::CommandBuffer> create_command_buffer
    (
        const vk::Device&
        , const vk::CommandPool&
        , const std::vector<vk::UniqueFramebuffer>&
        , const vk::RenderPass&
        , const vk::Extent2D&
        , const vk::Pipeline&
        , const vk::Buffer& vertices
        , const vk::Buffer& indices
        , const size_t indices_count
    );
}
