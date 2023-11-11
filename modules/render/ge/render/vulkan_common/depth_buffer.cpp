#include "depth_buffer.h"
#include "format.h"

namespace ge
{
    DepthBuffer DepthBuffer::create
    (
        const DeviceData& device_data,
        const Extent<size_t>& extent,
        const vk::CommandPool& command_pool,
        const vk::Fence& fence
    )
    {
        const vk::Format format = find_depth_format(device_data.physical_device_data.physical_device);
        ImageData image_data = ImageData::create
        (
            device_data,
            extent,
            format,
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
