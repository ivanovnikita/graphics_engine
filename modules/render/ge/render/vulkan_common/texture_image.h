#pragma once

#include "device.h"

#include "ge/io/image.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct TextureImageData final
    {
        static TextureImageData create_from_image
        (
            const DeviceData&,
            const vk::CommandPool&,
            const vk::Fence&,
            const Image&
        );

        vk::UniqueImage image;
        vk::UniqueDeviceMemory device_memory;
        vk::UniqueImageView image_view;
        vk::UniqueSampler sampler;
    };
}
