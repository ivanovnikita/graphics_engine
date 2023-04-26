#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        uint32_t size
    );
}
