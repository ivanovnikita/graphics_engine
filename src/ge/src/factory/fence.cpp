#include "fence.h"

namespace ge::impl::factory::fence
{
    vk::UniqueFence create
    (
        const vk::Device& logical_device
    )
    {
        const auto fence_info = vk::FenceCreateInfo{};
        return logical_device.createFenceUnique(fence_info);
    }
}
