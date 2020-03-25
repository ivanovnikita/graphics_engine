#pragma once

#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

#include <span>

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

    void copy_buffer
    (
        const vk::Device&
        , const vk::CommandPool&
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const vk::Buffer& src
        , const vk::Buffer& dst
        , const size_t size
    );

    template <typename BufferElement>
    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> create_and_fill_buffer
    (
        const vk::PhysicalDevice&
        , const vk::Device&
        , const vk::CommandPool&
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const vk::BufferUsageFlagBits
        , const std::span<const BufferElement>
    );
}

#include "ge/render/factory/buffer_impl.hpp"
