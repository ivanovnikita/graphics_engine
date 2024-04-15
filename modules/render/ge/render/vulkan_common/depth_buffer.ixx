module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.depth_buffer;

import extent;
import vulkan_common.device_fwd;
import vulkan_common.image;

namespace ge
{
    export struct DepthBuffer final
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
