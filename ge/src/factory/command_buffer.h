#pragma once

#include "command_pool.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::command_buffer
{
    std::vector<vk::CommandBuffer> create
    (
        const vk::Device&
        , const vk::CommandPool&
        , const std::vector<vk::UniqueFramebuffer>&
        , const vk::RenderPass&
        , const vk::Extent2D&
        , const vk::Pipeline&
    );
}
