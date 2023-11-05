#include "polygons_in_device_memory.h"

#include "ge/common/safe_cast.hpp"

namespace ge::tiles
{
    PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>& polygons
    )
    {
        static_assert(sizeof(World2dCoords) == 2 * sizeof(float));
        static_assert(sizeof(Color) == 3 * sizeof(float));

        size_t triangle_points_memory_usage = 0;
        size_t triangle_colors_memory_usage = 0;
        size_t line_points_memory_usage = 0;
        size_t line_colors_memory_usage = 0;

        PolygonsInDeviceMemory result;
        result.triangle_points_count = 0;
        result.line_points_count = 0;
        for (const Polygons& polygon : polygons)
        {
            triangle_points_memory_usage += 3 * sizeof(World2dCoords) * polygon.triangles.size();
            triangle_colors_memory_usage += 3 * sizeof(Color) * polygon.triangles.size();
            line_points_memory_usage += 2 * sizeof(World2dCoords) * polygon.lines.size();
            line_colors_memory_usage += 2 * sizeof(Color) * polygon.lines.size();

            result.triangle_points_count += 3 * polygon.triangles.size();
            result.line_points_count += 2 * polygon.lines.size();
        }

        const vk::DeviceSize buffer_size = safe_cast<vk::DeviceSize>
        (
            triangle_points_memory_usage +
            triangle_colors_memory_usage +
            line_points_memory_usage +
            line_colors_memory_usage
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

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::Triangle& triangle : polygon.triangles)
            {
                for (const size_t ind : triangle.inds)
                {
                    std::memcpy(current_offset, &polygon.points[ind], sizeof(World2dCoords));
                    current_offset += sizeof(World2dCoords);
                }
            }
        }

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::Line& line : polygon.lines)
            {
                for (const size_t ind : line.inds)
                {
                    std::memcpy(current_offset, &polygon.points[ind], sizeof(World2dCoords));
                    current_offset += sizeof(World2dCoords);
                }
            }
        }

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::Triangle& triangle : polygon.triangles)
            {
                for (size_t i = 0; i < 3; ++i)
                {
                    std::memcpy(current_offset, &triangle.color, sizeof(Color));
                    current_offset += sizeof(Color);
                }
            }
        }

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::Line& line : polygon.lines)
            {
                for (size_t i = 0; i < 2; ++i)
                {
                    std::memcpy(current_offset, &line.color, sizeof(Color));
                    current_offset += sizeof(Color);
                }
            }
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

        result.buffer = std::move(device_buffer);
        result.triangle_points_offset = 0;
        result.line_points_offset = result.triangle_points_offset + triangle_points_memory_usage;
        result.triangle_colors_offset = result.line_points_offset + line_points_memory_usage;
        result.line_colors_offset = result.triangle_colors_offset + triangle_colors_memory_usage;

        return result;
    }
}
