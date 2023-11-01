#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
