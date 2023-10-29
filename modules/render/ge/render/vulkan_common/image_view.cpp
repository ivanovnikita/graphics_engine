#include "image_view.h"
#include "exception.h"

namespace ge
{
    vk::UniqueImageView create_image_view
    (
        const vk::Device& device,
        const vk::Image& image,
        const vk::Format format
    )
    {
        const vk::ImageSubresourceRange subresource_range = vk::ImageSubresourceRange{}
            .setAspectMask(vk::ImageAspectFlagBits::eColor)
            .setBaseMipLevel(0)
            .setLevelCount(1)
            .setBaseArrayLayer(0)
            .setLayerCount(1);

        vk::ImageViewCreateInfo create_info = vk::ImageViewCreateInfo{}
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(format)
            .setSubresourceRange(subresource_range);

        create_info.setImage(image);

        vk::ImageView view;
        const vk::Result result = device.createImageView
        (
            &create_info,
            nullptr,
            &view
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
            GE_THROW_EXPECTED_RESULT(result, "Image view creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Image view creation failed");
        }
        }

        return vk::UniqueImageView
        {
            std::move(view),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }

    std::vector<vk::UniqueImageView> create_image_views
    (
        const vk::Device& device,
        const std::vector<vk::Image>& images,
        const vk::Format format
    )
    {
        std::vector<vk::UniqueImageView> views;
        try
        {
            views.reserve(images.size());
        }
        catch(const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for image views failed");
        }

        for (const vk::Image& image : images)
        {
            views.emplace_back(create_image_view(device, image, format));
        }

        return views;
    }
}
