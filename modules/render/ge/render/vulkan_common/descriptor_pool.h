#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size,
        std::span<const vk::DescriptorPoolSize>
    );
}
