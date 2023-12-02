#pragma once

#include "device.h"
#include "image.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct DepthBuffer final
    {
        static DepthBuffer create
        (
            const DeviceData&,
            const Extent<uint32_t>&,
            vk::SampleCountFlagBits,
            const vk::CommandPool&,
            const vk::Fence&
        );

        ImageData image_data;
        vk::Format format;
    };
}
