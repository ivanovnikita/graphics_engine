#pragma once

#include "queue.h"
#include "factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::physical
{

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create
    (
        const OptionsDevice&
      , const vk::Instance&
      , const vk::SurfaceKHR&
    );

}
