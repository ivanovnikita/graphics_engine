#pragma once

#include "render/queue.h"
#include "render/factory/options.h"

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
