#include "texture_image.h"
#include "buffer.h"
#include "exception.h"
#include "command_buffer.h"
#include "image_view.h"
#include "sampler.h"

namespace ge
{
    namespace
    {
        vk::UniqueImage create_image
        (
            const vk::Device& device,
            const size_t width,
            const size_t height,
            const vk::Format& format,
            const vk::ImageTiling& tiling,
            const vk::ImageUsageFlags& usage
        )
        {
            const vk::Extent3D extent = vk::Extent3D{}
                .setWidth(static_cast<uint32_t>(width))
                .setHeight(static_cast<uint32_t>(height))
                .setDepth(1);

            const vk::ImageCreateInfo imageInfo = vk::ImageCreateInfo{}
                .setImageType(vk::ImageType::e2D)
                .setExtent(extent)
                .setMipLevels(1)
                .setArrayLayers(1)
                .setFormat(format)
                .setTiling(tiling)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setUsage(usage)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setSharingMode(vk::SharingMode::eExclusive);

            vk::Image image;
            const vk::Result result = device.createImage(&imageInfo, nullptr, &image);
            switch (result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorCompressionExhaustedEXT:
            case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
                GE_THROW_EXPECTED_RESULT(result, "Image creation failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result, "Image creation failed");
            }
            }

            return vk::UniqueImage
            {
                std::move(image),
                vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
            };
        }

        TextureImageData create_empty
        (
            const DeviceData& device_data,
            const size_t width,
            const size_t height,
            const vk::Format& format,
            const vk::ImageTiling& tiling,
            const vk::ImageUsageFlags& usage,
            const vk::MemoryPropertyFlags& mem_properties
        )
        {
            const vk::Device& device = *device_data.logical_device;

            vk::UniqueImage image = create_image
            (
                device,
                width,
                height,
                format,
                tiling,
                usage
            );

            const vk::MemoryRequirements mem_requirements = device.getImageMemoryRequirements(*image);

            vk::UniqueDeviceMemory memory = allocate_device_memory
            (
                device_data,
                mem_properties,
                mem_requirements
            );

            const vk::Result result = vk::Result{vkBindImageMemory(device, *image, *memory, 0)};
            switch (result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(result, "Binding device memory to image failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result, "Binding device memory to image failed");
            }
            }

            vk::UniqueImageView image_view = create_image_view(device, *image, format);
            vk::UniqueSampler sampler = create_sampler(device_data);

            return TextureImageData
            {
                .image = std::move(image),
                .device_memory = std::move(memory),
                .image_view = std::move(image_view),
                .sampler = std::move(sampler)
            };
        }

        void transition_image_layout
        (
            const DeviceData& device_data,
            const vk::CommandPool& command_pool,
            const vk::Fence& fence,
            const vk::Image& image,
            const vk::ImageLayout& old_layout,
            const vk::ImageLayout& new_layout
        )
        {
            vk::UniqueCommandBuffer command_buffer = create_one_time_commands
            (
                *device_data.logical_device,
                command_pool
            );

            vk::ImageSubresourceRange subresource_range = vk::ImageSubresourceRange{}
                .setAspectMask(vk::ImageAspectFlagBits::eColor)
                .setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1);

            vk::ImageMemoryBarrier barrier = vk::ImageMemoryBarrier{}
                .setOldLayout(old_layout)
                .setNewLayout(new_layout)
                .setSrcQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setDstQueueFamilyIndex(VK_QUEUE_FAMILY_IGNORED)
                .setImage(image)
                .setSubresourceRange(std::move(subresource_range));

            vk::PipelineStageFlags source_stage;
            vk::PipelineStageFlags destination_stage;

            if
            (
                old_layout == vk::ImageLayout::eUndefined and
                new_layout == vk::ImageLayout::eTransferDstOptimal
            )
            {
                barrier.setSrcAccessMask(vk::AccessFlags{});
                barrier.setDstAccessMask(vk::AccessFlagBits::eTransferWrite);

                source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
                destination_stage = vk::PipelineStageFlagBits::eTransfer;
            }
            else if
            (
                old_layout == vk::ImageLayout::eTransferDstOptimal and
                new_layout == vk::ImageLayout::eShaderReadOnlyOptimal
            )
            {
                barrier.setSrcAccessMask(vk::AccessFlagBits::eTransferWrite);
                barrier.setDstAccessMask(vk::AccessFlagBits::eShaderRead);

                source_stage = vk::PipelineStageFlagBits::eTransfer;
                destination_stage = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else
            {
                __builtin_unreachable();
            }

            command_buffer->pipelineBarrier
            (
                source_stage,
                destination_stage,
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

        TextureImageData image_data = create_empty
        (
            device_data,
            image.x(),
            image.y(),
            format,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
            vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal
        );

        copy_buffer_to_image
        (
            device_data,
            command_pool,
            fence,
            *staging_buffer.buffer,
            *image_data.image,
            image.x(),
            image.y()
        );

        transition_image_layout
        (
            device_data,
            command_pool,
            fence,
            *image_data.image,
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::eShaderReadOnlyOptimal
        );

        return image_data;
    }
}
