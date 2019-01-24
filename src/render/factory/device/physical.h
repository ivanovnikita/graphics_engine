#pragma once

#include "queue.h"
#include "factory/options.h"

#include <vulkan/vulkan.hpp>

#include <optional>

namespace ge::factory
{
    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create_physical_device
    (
        const options::Device&
      , const vk::Instance&
      , const std::optional<vk::SurfaceKHR>& = std::optional<vk::SurfaceKHR>()
    );
}
