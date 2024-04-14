module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.semaphore;

namespace ge
{
    export vk::UniqueSemaphore create_semaphore(const vk::Device&);
}
