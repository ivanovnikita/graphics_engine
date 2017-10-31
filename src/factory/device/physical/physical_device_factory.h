#pragma once

#include <vulkan/vulkan.hpp>
#include "queue.h"

namespace ge::impl::factory::device::physical
{

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create(const vk::Instance&, const vk::SurfaceKHR&);

} // namespace ge::impl::factory::device::physical
