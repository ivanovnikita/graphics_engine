#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::logical
{

    template<typename Layers>
    vk::UniqueDevice create
    (
        const vk::PhysicalDevice& physical_device
      , const Layers& required_layers
      , const vk::SurfaceKHR& surface
    );

} // namespace ge::impl::factory::device::logical

#include "logical_device_factory_impl.hpp"
