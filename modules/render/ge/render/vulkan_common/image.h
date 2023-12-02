#pragma once

#include "device.h"
#include "ge/render/extent.h"

namespace ge
{
    struct ImageData final
    {
        static ImageData create
        (
            const DeviceData&,
            const Extent<uint32_t>&,
            const vk::Format&,
            const uint32_t mip_levels,
            const vk::SampleCountFlagBits&,
            const vk::ImageTiling&,
            const vk::ImageUsageFlags&,
            const vk::MemoryPropertyFlags&,
            const vk::ImageAspectFlags&
        );

        vk::UniqueImage image;
        vk::UniqueDeviceMemory device_memory;
        vk::UniqueImageView image_view;
    };

    void transition_image_layout
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence&,
        const vk::Image&,
        const vk::Format&,
        const uint32_t mip_levels,
        const vk::ImageLayout&,
        const vk::ImageLayout&
    );
}
