module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.texture_image;

export import vulkan_common.image;
export import io.image;

import vulkan_common.device_fwd;

namespace ge
{
    export struct TextureImageData final
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
