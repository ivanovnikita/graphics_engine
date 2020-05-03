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

    /*
        Graph in memory:
        [arc points: [vec2; vec2], ...]
        [vertice points: [vec2, ...]]
        [arc colors: [u8vec2; u8vec2], ...] // (duplicated for each vertice in arc)
        [vertice colors: [u8vec, ...]]
    */
    struct GraphInDeviceMemory final
    {
        vk::UniqueDeviceMemory memory;
        vk::UniqueBuffer buffer;
        vk::DeviceSize arc_points_offset;
        vk::DeviceSize vertice_points_offset;
        vk::DeviceSize arc_colors_offset;
        vk::DeviceSize vertice_colors_offset;
        size_t arc_points_count;
        size_t vertice_points_count;
    };

    GraphInDeviceMemory load_graph_to_device
    (
        const vk::PhysicalDevice&
        , const vk::Device&
        , const vk::CommandPool&
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const Graph&
    );

    // TODO: add func for partial update of colors
    // TODO: add func for full update of colors
}

#include "ge/render/factory/buffer_impl.hpp"
