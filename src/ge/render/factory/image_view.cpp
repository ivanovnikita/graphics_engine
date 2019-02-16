#include "ge/render/factory/image_view.h"

namespace ge::factory
{
    std::vector<vk::UniqueImageView> create_image_view
    (
        const std::vector<vk::Image>& images
      , vk::Format format
      , const vk::Device& logical_device
    )
    {
        std::vector<vk::UniqueImageView> image_views;
        image_views.reserve(images.size());

        const auto subresource_range = vk::ImageSubresourceRange{}
            .setAspectMask(vk::ImageAspectFlagBits::eColor)
            .setBaseMipLevel(0)
            .setLevelCount(1)
            .setBaseArrayLayer(0)
            .setLayerCount(1);

        auto create_info = vk::ImageViewCreateInfo{}
            .setViewType(vk::ImageViewType::e2D)
            .setFormat(format)
            .setSubresourceRange(subresource_range);

        for (const auto& image : images)
        {
            create_info.setImage(image);
            image_views.emplace_back(logical_device.createImageViewUnique(create_info));
        }

        return image_views;
    }
}
