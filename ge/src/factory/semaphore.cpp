#include "semaphore.h"

namespace ge::impl::factory::semaphore
{
    vk::UniqueSemaphore create
    (
        const vk::Device& logical_device
    )
    {
        const auto create_info = vk::SemaphoreCreateInfo{};
        return logical_device.createSemaphoreUnique(create_info);
    }
}
