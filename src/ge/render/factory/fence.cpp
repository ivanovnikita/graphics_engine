#include "ge/render/factory/fence.h"

namespace ge::factory
{
    vk::UniqueFence create_fence
    (
        const vk::Device& logical_device
    )
    {
        const auto fence_info = vk::FenceCreateInfo{};
        return logical_device.createFenceUnique(fence_info);
    }
}
