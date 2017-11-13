#pragma once

#include "queue.h"
#include "options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    vk::UniqueDevice create(const OptionsDevice&, const vk::PhysicalDevice&, QueueFamilyIndices);

} // namespace ge::impl::factory::device::logical
