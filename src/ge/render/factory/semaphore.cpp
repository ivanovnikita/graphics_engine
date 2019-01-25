#include "ge/render/factory/semaphore.h"

namespace ge::factory
{
    vk::UniqueSemaphore create_semaphore
    (
        const vk::Device& logical_device
    )
    {
        const auto create_info = vk::SemaphoreCreateInfo{};
        return logical_device.createSemaphoreUnique(create_info);
    }
}
