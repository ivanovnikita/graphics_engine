#pragma once

#include "window/window.h"
#include "queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::swapchain
{

    std::pair<vk::UniqueSwapchainKHR, vk::Format> create
    (
        const vk::PhysicalDevice&
      , const vk::Device&
      , const Window&
      , const vk::SurfaceKHR&
      , const QueueFamilyIndices&
    );

} // namespace ge::impl::factory::swapchain
