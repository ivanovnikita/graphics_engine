#include "command_pool.h"
#include "exception.h"

namespace ge::factory
{
    vk::UniqueCommandPool create_command_pool
    (
        const vk::Device& logical_device
        , const QueueFamilyIndices& queue_family_indices
    )
    {
        if (not queue_family_indices.graphics.has_value())
        {
            GE_THROW(device_capabilities_error, "Graphics queue is unavailable");
        }

        const auto pool_info = vk::CommandPoolCreateInfo()
            .setQueueFamilyIndex(*queue_family_indices.graphics);

        return logical_device.createCommandPoolUnique(pool_info);
    }
}
