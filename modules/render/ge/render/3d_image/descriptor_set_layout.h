#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
