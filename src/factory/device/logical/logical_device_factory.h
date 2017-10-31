#pragma once

#include "queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    vk::UniqueDevice create(const vk::PhysicalDevice&, QueueFamilyIndices);

} // namespace ge::impl::factory::device::logical
