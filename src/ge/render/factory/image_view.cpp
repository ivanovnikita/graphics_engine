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

        const uint32_t base_mip_level = 0;
        const uint32_t level_count = 1;
        const uint32_t base_array_layer = 0;
        const uint32_t layer_count = 1;

        for (const auto& image : images)
        {
            const vk::ImageViewCreateInfo create_info
            {
                vk::ImageViewCreateFlags{}
              , image
              , vk::ImageViewType::e2D
              , format
              , vk::ComponentMapping{}
              , vk::ImageSubresourceRange
                {
                    vk::ImageAspectFlagBits::eColor
                  , base_mip_level
                  , level_count
                  , base_array_layer
                  , layer_count
                }
            };

            image_views.emplace_back(logical_device.createImageViewUnique(create_info));
        }

        return image_views;
    }
}
