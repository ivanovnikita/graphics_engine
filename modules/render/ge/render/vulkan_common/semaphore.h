#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueSemaphore create_semaphore(const vk::Device&);
}
