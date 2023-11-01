#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}
