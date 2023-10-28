#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayoutCreateInfo&
    );
}
