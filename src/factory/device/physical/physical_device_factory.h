#pragma once

#include "queue.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::physical
{

    struct OptionGraphics
    {
        bool enabled = false;
    };

    struct OptionCompute
    {
        bool enabled = false;
    };

    struct OptionTransfer
    {
        bool enabled = false;
    };

    struct OptionsPhysicalDevice
    {
        OptionGraphics graphics;
        OptionCompute compute;
        OptionTransfer transfer;
    };

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create
    (
        const OptionsPhysicalDevice&
      , const vk::Instance&
      , const vk::SurfaceKHR&
    );

} // namespace ge::impl::factory::device::physical
