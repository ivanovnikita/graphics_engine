#pragma once

#include "queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::command_pool
{
    vk::UniqueCommandPool create
    (
        const vk::Device&
        , const QueueFamilyIndices&
    );
}
