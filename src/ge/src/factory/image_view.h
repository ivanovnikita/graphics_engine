#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::image_view
{

    std::vector<vk::UniqueImageView> create(const std::vector<vk::Image>&, vk::Format, const vk::Device&);

}
