#pragma once

#include "render/queue.h"
#include "render/factory/options.h"

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
