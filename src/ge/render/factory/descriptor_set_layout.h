#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueDescriptorSetLayout camera_2d_descriptor_set_layout
    (
        const vk::Device&
    );
}
