#pragma once

#include "ge/render/queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueCommandPool create_command_pool
    (
        const vk::Device&
        , const QueueFamilyIndices&
    );
}
