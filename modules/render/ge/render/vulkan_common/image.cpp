#include "image.h"
#include "exception.h"
#include "buffer.h"
#include "command_buffer.h"
#include "image_view.h"
#include "format.h"

namespace ge
{
    namespace
    {
        vk::UniqueImage create_image
        (
            const vk::Device& device,
            const Extent<uint32_t>& extent,
            const vk::Format& format,
            const vk::ImageTiling& tiling,
            const vk::ImageUsageFlags& usage,
            const uint32_t mip_levels
        )
        {
            const vk::Extent3D extent_3d = vk::Extent3D{}
                .setWidth(extent.width)
                .setHeight(extent.height)
                .setDepth(1);

            const vk::ImageCreateInfo imageInfo = vk::ImageCreateInfo{}
                .setImageType(vk::ImageType::e2D)
                .setExtent(extent_3d)
                .setMipLevels(mip_levels)
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
    }

    ImageData ImageData::create
    (
        const DeviceData& device_data,
        const Extent<uint32_t>& extent,
        const vk::Format& format,
        const uint32_t mip_levels,
        const vk::ImageTiling& tiling,
        const vk::ImageUsageFlags& usage,
        const vk::MemoryPropertyFlags& mem_properties,
        const vk::ImageAspectFlags& aspect
    )
    {
        const vk::Device& device = *device_data.logical_device;

        vk::UniqueImage image = create_image
        (
            device,
            extent,
            format,
            tiling,
            usage,
            mip_levels
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

        vk::UniqueImageView image_view = create_image_view(device, *image, format, mip_levels, aspect);

        return ImageData
        {
            .image = std::move(image),
            .device_memory = std::move(memory),
            .image_view = std::move(image_view)
        };
    }

    void transition_image_layout
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& fence,
        const vk::Image& image,
        const vk::Format& format,
        const uint32_t mip_levels,
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
            .setLevelCount(mip_levels)
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

        if (new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal)
        {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;

            if (has_stencil_component(format))
            {
                barrier.subresourceRange.aspectMask |= vk::ImageAspectFlagBits::eStencil;
            }
        }
        else
        {
            barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        }

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
        else if
        (
            old_layout == vk::ImageLayout::eUndefined and
            new_layout == vk::ImageLayout::eDepthStencilAttachmentOptimal
        )
        {
            barrier.setSrcAccessMask(vk::AccessFlags{});
            barrier.setDstAccessMask
            (
                vk::AccessFlagBits::eDepthStencilAttachmentRead |
                vk::AccessFlagBits::eDepthStencilAttachmentWrite
            );

            source_stage = vk::PipelineStageFlagBits::eTopOfPipe;
            destination_stage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
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
}
