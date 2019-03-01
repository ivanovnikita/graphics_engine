#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> create_buffer
    (
        const vk::PhysicalDevice&
        , const vk::Device&
        , const size_t size
        , const vk::BufferUsageFlags
        , const vk::MemoryPropertyFlags
    );
}
