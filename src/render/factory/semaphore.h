#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueSemaphore create_semaphore
    (
        const vk::Device&
    );
}

