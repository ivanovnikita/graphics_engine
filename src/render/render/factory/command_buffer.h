#pragma once

#include "render/factory/command_pool.h"

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
    );
}
