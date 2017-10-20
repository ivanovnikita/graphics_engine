#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    template<typename Layers>
    vk::UniqueDevice create
    (
        const vk::PhysicalDevice& physical_device
      , const Layers& required_layers
      , uint32_t queue_family_index
    );

} // namespace ge::impl::factory::device::logical

#include "logical_device_factory_impl.hpp"
