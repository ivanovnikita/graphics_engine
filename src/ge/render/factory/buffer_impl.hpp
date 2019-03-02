#pragma once

#include "ge/render/factory/buffer.hpp"
#include "ge/render/utils/safe_cast.hpp"

namespace ge::factory
{
    template <typename BufferElement>
    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> create_and_fill_buffer
    (
        const vk::PhysicalDevice& physical_device
        , const vk::Device& logical_device
        , const vk::CommandPool& command_pool
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const vk::BufferUsageFlagBits usage
        , const Span<const BufferElement> elements
    )
    {
        const uint32_t buffer_size = safe_cast<uint32_t>(sizeof(BufferElement) * elements.size());

        auto [staging_buffer, staging_memory] = factory::create_buffer
        (
            physical_device
            , logical_device
            , buffer_size
            , vk::BufferUsageFlagBits::eTransferSrc
            , vk::MemoryPropertyFlagBits::eHostVisible
            | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        constexpr uint32_t offset = 0;
        void* data = logical_device.mapMemory
        (
            *staging_memory
            , offset
            , buffer_size
            , vk::MemoryMapFlags{}
        );
        std::memcpy(data, elements.data(), static_cast<size_t>(buffer_size));
        logical_device.unmapMemory(*staging_memory);

        auto [vertex_buffer, vertex_memory] = factory::create_buffer
        (
            physical_device
            , logical_device
            , buffer_size
            , vk::BufferUsageFlagBits::eTransferDst | usage
            , vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        copy_buffer
        (
            logical_device
            , command_pool
            , transfer
            , transfer_finished
            , *staging_buffer
            , *vertex_buffer
            , buffer_size
        );

        return {std::move(vertex_buffer), std::move(vertex_memory)};
    }
}
