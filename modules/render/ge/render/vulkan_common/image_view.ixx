module;

#include <vulkan/vulkan.hpp>

#include <span>

export module vulkan_common.image_view;

namespace ge
{
    export vk::UniqueImageView create_image_view
    (
        const vk::Device&,
        const vk::Image&,
        const vk::Format,
        const uint32_t mip_levels,
        const vk::ImageAspectFlags&
    );
    export std::vector<vk::UniqueImageView> create_image_views
    (
        const vk::Device&,
        const std::span<vk::Image>&,
        const vk::Format,
        const uint32_t mip_levels,
        const vk::ImageAspectFlags&
    );
}
