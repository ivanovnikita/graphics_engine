#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}
