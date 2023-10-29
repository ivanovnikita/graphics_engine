#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueImageView create_image_view
    (
        const vk::Device&,
        const vk::Image&,
        const vk::Format
    );
    std::vector<vk::UniqueImageView> create_image_views
    (
        const vk::Device& device,
        const std::vector<vk::Image>& images,
        const vk::Format format
    );
}
