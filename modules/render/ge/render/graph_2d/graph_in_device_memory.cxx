module;

#include <vulkan/vulkan.hpp>

module graph_2d.graph_in_device_memory;

import safe_cast;

namespace ge::graph
{
    GraphInDeviceMemory load_graph_to_device
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& transfer_finished,
        const Graph& graph
    )
    {
        static_assert(sizeof(World2dCoords) == 2 * sizeof(float));
        static_assert(sizeof(Color) == 3 * sizeof(float));

        const size_t arc_points_memory_usage = sizeof(World2dCoords) * graph.arcs.size() * 2;
        const size_t vertice_points_memory_usage = sizeof(World2dCoords) * graph.vertices.size();
        const size_t arc_colors_memory_usage = sizeof(Color) * graph.arcs.size() * 2;
        const size_t vertice_colors_memory_usage = sizeof(Color) * graph.vertices.size();

        const vk::DeviceSize buffer_size = safe_cast<vk::DeviceSize>
        (
            arc_points_memory_usage +
            vertice_points_memory_usage +
            arc_colors_memory_usage +
            vertice_colors_memory_usage
        );

        BufferData staging_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent,
            buffer_size
        );

        constexpr vk::DeviceSize offset = 0;
        void* const memory_start = map_memory
        (
            *device_data.logical_device,
            *staging_buffer.device_memory,
            offset,
            buffer_size,
            vk::MemoryMapFlags{}
        );

        uint8_t* current_offset = static_cast<uint8_t*>(memory_start);

        for (const Graph::Arc& arc : graph.arcs)
        {
            std::memcpy(current_offset, &graph.points[arc.index_from], sizeof(World2dCoords));
            current_offset += sizeof(World2dCoords);

            std::memcpy(current_offset, &graph.points[arc.index_to], sizeof(World2dCoords));
            current_offset += sizeof(World2dCoords);
        }

        for (const Graph::Vertice& vertice : graph.vertices)
        {
            std::memcpy(current_offset, &graph.points[vertice.index], sizeof(World2dCoords));
            current_offset += sizeof(World2dCoords);
        }

        for (const Graph::Arc& arc : graph.arcs)
        {
            std::memcpy(current_offset, &arc.color, sizeof(Color));
            current_offset += sizeof(Color);
            std::memcpy(current_offset, &arc.color, sizeof(Color));
            current_offset += sizeof(Color);
        }

        for (const Graph::Vertice& vertice : graph.vertices)
        {
            std::memcpy(current_offset, &vertice.color, sizeof(Color));
            current_offset += sizeof(Color);
        }

        assert(current_offset == static_cast<uint8_t*>(memory_start) + buffer_size);

        device_data.logical_device->unmapMemory(*staging_buffer.device_memory);

        BufferData device_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            buffer_size
        );

        copy_buffer
        (
            device_data
            , command_pool
            , transfer_finished
            , *staging_buffer.buffer
            , *device_buffer.buffer
            , buffer_size
        );

        GraphInDeviceMemory result;
        result.buffer = std::move(device_buffer);
        result.arc_points_offset = 0;
        result.vertice_points_offset = result.arc_points_offset + arc_points_memory_usage;
        result.arc_colors_offset = result.vertice_points_offset + vertice_points_memory_usage;
        result.vertice_colors_offset = result.arc_colors_offset + arc_colors_memory_usage;
        result.arc_points_count = graph.arcs.size() * 2;
        result.vertice_points_count = graph.vertices.size();

        return result;
    }
}
