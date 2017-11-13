#pragma once

#include "queue.h"
#include "options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::physical
{

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create
    (
        const OptionsDevice&
      , const vk::Instance&
      , const vk::SurfaceKHR&
    );

} // namespace ge::impl::factory::device::physical
