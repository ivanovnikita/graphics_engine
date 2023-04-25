#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct SwapchainData final
    {
        static SwapchainData create_default
        (
            const DeviceData&,
            const vk::Extent2D&,
            const vk::SurfaceKHR&
        );

        vk::UniqueSwapchainKHR swapchain;
        vk::Format format;
        vk::Extent2D extent;
    };
}
