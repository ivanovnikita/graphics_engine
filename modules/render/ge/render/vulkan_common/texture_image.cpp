#include "texture_image.h"
#include "buffer.h"
#include "command_buffer.h"
#include "sampler.h"

namespace ge
{
    namespace
    {
        void copy_buffer_to_image
        (
            const DeviceData& device_data,
            const vk::CommandPool& command_pool,
            const vk::Fence& fence,
            const vk::Buffer& buffer,
            const vk::Image& image,
            const size_t width,
            const size_t height
        )
        {
            vk::UniqueCommandBuffer command_buffer = create_one_time_commands
            (
                *device_data.logical_device,
                command_pool
            );

            vk::ImageSubresourceLayers image_subresource = vk::ImageSubresourceLayers{}
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setMipLevel(0)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

            const vk::BufferImageCopy region = vk::BufferImageCopy{}
                .setBufferOffset(0)
                .setBufferRowLength(0)
                .setBufferImageHeight(0)
                .setImageSubresource(std::move(image_subresource))
                .setImageOffset(vk::Offset3D{0, 0, 0})
                .setImageExtent
                (
                    vk::Extent3D{}
                        .setWidth(static_cast<uint32_t>(width))
                        .setHeight(static_cast<uint32_t>(height))
                        .setDepth(1)
                );

            command_buffer->copyBufferToImage
            (
                buffer,
                image,
                vk::ImageLayout::eTransferDstOptimal,
                1,
                &region
            );

            submit_one_time_commands(std::move(command_buffer), device_data, fence);
        }
    }

    TextureImageData TextureImageData::create_from_image
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& fence,
        const Image& image
    )
    {
        BufferData staging_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent,
            image.bytes_count()
        );

        constexpr vk::DeviceSize offset = 0;
        void* const memory_start = map_memory
        (
            *device_data.logical_device,
            *staging_buffer.device_memory,
            offset,
            image.bytes_count(),
            vk::MemoryMapFlags{}
        );

        uint8_t* current_offset = static_cast<uint8_t*>(memory_start);

        std::memcpy(current_offset, image.data(), image.bytes_count());

        device_data.logical_device->unmapMemory(*staging_buffer.device_memory);

        const vk::Format format = vk::Format::eR8G8B8A8Srgb;

        TextureImageData image_data
        {
            .image_data = ImageData::create
            (
                device_data,
                Extent<size_t>{.width = image.x(), .height = image.y()},
                format,
                vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                vk::ImageAspectFlagBits::eColor
            ),
            .sampler = create_sampler(device_data)
        };

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image_data.image,
            format,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal
        );

        copy_buffer_to_image
        (
            device_data,
            command_pool,
            fence,
            *staging_buffer.buffer,
            *image_data.image_data.image,
            image.x(),
            image.y()
        );

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image_data.image,
            format,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal
        );

        return image_data;
    }
}
