#pragma once

#include "queue.h"
#include "factory/options.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueDevice create_logical_device
    (
        const options::ValidationLayers&
        , const vk::PhysicalDevice&
        , const QueueFamilyIndices&
    );
}
