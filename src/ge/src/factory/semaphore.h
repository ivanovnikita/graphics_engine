#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::semaphore
{
    vk::UniqueSemaphore create
    (
        const vk::Device&
    );
}

