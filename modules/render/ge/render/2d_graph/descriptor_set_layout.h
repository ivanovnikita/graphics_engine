#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
