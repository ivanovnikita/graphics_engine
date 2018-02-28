#pragma once

#include "queue.h"
#include "factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    vk::UniqueDevice create(const options::Device&, const vk::PhysicalDevice&, const QueueFamilyIndices&);

}
