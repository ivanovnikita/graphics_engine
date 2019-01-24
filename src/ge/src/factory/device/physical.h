#pragma once

#include "queue.h"
#include "factory/options.h"

#include <vulkan/vulkan.hpp>

#include <optional>

namespace ge::impl::factory::device::physical
{

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create
    (
        const options::Device&
      , const vk::Instance&
      , const std::optional<vk::SurfaceKHR>& = std::optional<vk::SurfaceKHR>()
    );

}
