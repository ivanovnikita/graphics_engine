#include "texture_image.h"
#include "buffer.h"
#include "command_buffer.h"
#include "sampler.h"

#include "ge/common/safe_cast.hpp"

#include <cmath>

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
            const Extent<uint32_t>& image_extent
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
                        .setWidth(image_extent.width)
                        .setHeight(image_extent.height)
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

        void generate_mipmaps
        (
            const DeviceData& device_data,
            const vk::CommandPool& command_pool,
            const vk::Fence& fence,
            const vk::Image& image,
            const vk::Format format,
            const Extent<uint32_t> extent,
            const uint32_t mip_levels
        )
        {
            const vk::FormatProperties format_properties = device_data
                .physical_device_data
                .physical_device
                .getFormatProperties(format);
            if (not (format_properties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImageFilterLinear))
            {
                GE_THROW_EXPECTED_ERROR("Texture image format does not support linear blitting");
            }

            vk::UniqueCommandBuffer command_buffer = create_one_time_commands
            (
                *device_data.logical_device,
                command_pool
            );

            vk::ImageSubresourceRange subresource_range = vk::ImageSubresourceRange{}
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setLevelCount(1);

            vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier{}
                .setImage(image)
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setSubresourceRange(std::move(subresource_range));

            Extent<uint32_t> mip_extent = extent;
            for (uint32_t mip_level = 1; mip_level < mip_levels; ++mip_level)
            {
                barrier.subresourceRange.setBaseMipLevel(mip_level - 1);
                barrier.setOldLayout(vk::ImageLayout::eTransferDstOptimal);
                barrier.setNewLayout(vk::ImageLayout::eTransferSrcOptimal);
                barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
                barrier.setDstAccessMask(vk::AccessFlagBits::eTransferRead);

                command_buffer->pipelineBarrier
                (
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::DependencyFlags{},
                    0,
                    nullptr,
                    0,
                    nullptr,
                    1,
                    &barrier
                );

                const Extent<uint32_t> new_mip_extent
                {
                    .width = mip_extent.width > 1
                                ? mip_extent.width / 2
                                : 1,
                    .height = mip_extent.height > 1
                                ? mip_extent.height / 2
                                : 1
                };

                const vk::ImageBlit blit = vk::ImageBlit{}
                    .setSrcOffsets
                    (
                        {
                            vk::Offset3D{0, 0, 0},
                            vk::Offset3D
                            {
                                safe_cast<int32_t>(mip_extent.width),
                                safe_cast<int32_t>(mip_extent.height),
                                1
                            }
                        }
                    )
                    .setSrcSubresource
                    (
                        vk::ImageSubresourceLayers{}
                            .setAspectMask(vk::ImageAspectFlagBits::eColor)
                            .setMipLevel(mip_level - 1)
                            .setBaseArrayLayer(0)
                            .setLayerCount(1)
                    )
                    .setDstOffsets
                    (
                        {
                            vk::Offset3D{0, 0, 0},
                            vk::Offset3D
                            {
                                safe_cast<int32_t>(new_mip_extent.width),
                                safe_cast<int32_t>(new_mip_extent.height),
                                1
                            }
                        }
                    )
                    .setDstSubresource
                    (
                        vk::ImageSubresourceLayers{}
                            .setAspectMask(vk::ImageAspectFlagBits::eColor)
                            .setMipLevel(mip_level)
                            .setBaseArrayLayer(0)
                            .setLayerCount(1)
                    );

                command_buffer->blitImage
                (
                    image,
                    vk::ImageLayout::eTransferSrcOptimal,
                    image,
                    vk::ImageLayout::eTransferDstOptimal,
                    1,
                    &blit,
                    vk::Filter::eLinear
                );

                barrier.setOldLayout(vk::ImageLayout::eTransferSrcOptimal);
                barrier.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
                barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferRead);
                barrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);

                command_buffer->pipelineBarrier
                (
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eFragmentShader,
                    vk::DependencyFlags{},
                    0,
                    nullptr,
                    0,
                    nullptr,
                    1,
                    &barrier
                );

                mip_extent = new_mip_extent;
            }

            barrier.subresourceRange.setBaseMipLevel(mip_levels - 1);
            barrier.setOldLayout(vk::ImageLayout::eTransferDstOptimal);
            barrier.setNewLayout(vk::ImageLayout::eShaderReadOnlyOptimal);
            barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
            barrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);

            command_buffer->pipelineBarrier
            (
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::DependencyFlags{},
                0,
                nullptr,
                0,
                nullptr,
                1,
                &barrier
            );

            submit_one_time_commands(std::move(command_buffer), device_data, fence);
        }
    }

    TextureImageData TextureImageData::create_from_image
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& fence,
        const Image& image,
        const bool is_generate_mipmaps
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

        const uint32_t mip_levels = is_generate_mipmaps
            ? static_cast<uint32_t>(std::floor(std::log2(std::max(image.x(), image.y())))) + 1
            : 1;

        vk::ImageUsageFlags usage =
            vk::ImageUsageFlagBits::eTransferDst |
            vk::ImageUsageFlagBits::eSampled;

        if (is_generate_mipmaps)
        {
            usage = usage | vk::ImageUsageFlagBits::eTransferSrc;
        }

        const Extent<uint32_t> image_extent
        {
            .width = safe_cast<uint32_t>(image.x()),
            .height = safe_cast<uint32_t>(image.y())
        };
        TextureImageData image_data
        {
            .image_data = ImageData::create
            (
                device_data,
                image_extent,
                format,
                mip_levels,
                vk::SampleCountFlagBits::e1,
                vk::ImageTiling::eOptimal,
                usage,
                vk::MemoryPropertyFlagBits::eDeviceLocal,
                vk::ImageAspectFlagBits::eColor
            ),
            .sampler = create_sampler(device_data, mip_levels)
        };

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image_data.image,
            format,
            mip_levels,
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
            image_extent
        );

        if (is_generate_mipmaps)
        {
            generate_mipmaps
            (
                device_data,
                command_pool,
                fence,
                *image_data.image_data.image,
                format,
                image_extent,
                mip_levels
            );
        }
        else
        {
            transition_image_layout
            (
                device_data,
                command_pool,
                fence,
                *image_data.image_data.image,
                format,
                mip_levels,
                vk::ImageLayout::eTransferDstOptimal,
                vk::ImageLayout::eShaderReadOnlyOptimal
            );
        }

        return image_data;
    }
}
