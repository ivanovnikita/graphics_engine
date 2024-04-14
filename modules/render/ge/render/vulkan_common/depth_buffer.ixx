module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.depth_buffer;

export import vulkan_common.device;
export import vulkan_common.image;

import vulkan_common.format;

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

module : private;

namespace ge
{
    DepthBuffer DepthBuffer::create
    (
        const DeviceData& device_data,
        const Extent<uint32_t>& extent,
        const vk::SampleCountFlagBits sample_count,
        const vk::CommandPool& command_pool,
        const vk::Fence& fence
    )
    {
        const vk::Format format = find_depth_format(device_data.physical_device_data.physical_device);
        const uint32_t mip_levels = 1;
        ImageData image_data = ImageData::create
        (
            device_data,
            extent,
            format,
            mip_levels,
            sample_count,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vk::ImageAspectFlagBits::eDepth
        );

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image,
            format,
            mip_levels,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eDepthStencilAttachmentOptimal
        );

        return DepthBuffer
        {
            .image_data = std::move(image_data),
            .format = format
        };
    }
}