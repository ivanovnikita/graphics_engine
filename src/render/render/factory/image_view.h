#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::vector<vk::UniqueImageView> create_image_view(const std::vector<vk::Image>&, vk::Format, const vk::Device&);
}
