#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&
      , const size_t size
    );
}
