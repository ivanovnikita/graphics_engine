#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    vk::UniqueDevice create
    (
        const vk::PhysicalDevice& physical_device
      , uint32_t queue_family_index
    );

} // namespace ge::impl::factory::device::logical
