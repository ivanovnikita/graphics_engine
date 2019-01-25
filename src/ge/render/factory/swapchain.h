#pragma once

#include "ge/render/queue.h"

#include "ge/window/window.h"

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
