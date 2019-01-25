#pragma once

#include "ge/render/queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::pair<vk::UniqueSwapchainKHR, vk::Format> create_swapchain
    (
        const vk::PhysicalDevice&
      , const vk::Device&
      , const vk::Extent2D&
      , const vk::SurfaceKHR&
      , const QueueFamilyIndices&
    );
}
