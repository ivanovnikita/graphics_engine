#pragma once

#include "device.h"
#include "surface.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct SwapchainData final
    {
        static SwapchainData create_default
        (
            const DeviceData&,
            SurfaceData&
        );

        vk::UniqueSwapchainKHR swapchain;
        vk::Format format;
        vk::Extent2D extent;
        std::vector<vk::Image> images;
        std::vector<vk::UniqueImageView> image_views;
    };
}
