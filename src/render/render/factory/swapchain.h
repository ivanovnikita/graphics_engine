#pragma once

#include "render/window/window.h"
#include "render/queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::pair<vk::UniqueSwapchainKHR, vk::Format> create_swapchain
    (
        const vk::PhysicalDevice&
      , const vk::Device&
      , const Window&
      , const vk::SurfaceKHR&
      , const QueueFamilyIndices&
    );
}
