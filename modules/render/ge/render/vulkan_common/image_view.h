#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::UniqueImageView create_image_view
    (
        const vk::Device&,
        const vk::Image&,
        const vk::Format,
        const uint32_t mip_levels,
        const vk::ImageAspectFlags&
    );
    std::vector<vk::UniqueImageView> create_image_views
    (
        const vk::Device&,
        const std::span<vk::Image>&,
        const vk::Format,
        const uint32_t mip_levels,
        const vk::ImageAspectFlags&
    );
}
