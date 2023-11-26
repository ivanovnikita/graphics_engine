#pragma once

#include "image.h"

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
            const Image&,
            bool generate_mipmaps
        );

        ImageData image_data;
        vk::UniqueSampler sampler;
    };
}
