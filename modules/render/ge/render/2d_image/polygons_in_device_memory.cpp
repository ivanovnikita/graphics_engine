#include "polygons_in_device_memory.h"

#include "ge/common/safe_cast.hpp"

namespace ge::image
{
    PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>& polygons
    )
    {
        static_assert(sizeof(Vertex) == 2 * sizeof(float));
        static_assert(sizeof(TextureCoord) == 2 * sizeof(float));

        size_t triangle_points_memory_usage = 0;
        size_t texture_coords_memory_usage = 0;

        PolygonsInDeviceMemory result;
        result.vertices_count = 0;
        for (const Polygons& polygon : polygons)
        {
            triangle_points_memory_usage += 3 * sizeof(Vertex) * polygon.triangles.size();
            texture_coords_memory_usage += 3 * sizeof(TextureCoord) * polygon.triangles.size();
        }

        const vk::DeviceSize buffer_size = safe_cast<vk::DeviceSize>
        (
            triangle_points_memory_usage +
            texture_coords_memory_usage
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
                for (const Polygons::TexturedVertex& vertex : triangle.vertices)
                {
                    std::memcpy(current_offset, &polygon.points[vertex.index], sizeof(Vertex));
                    current_offset += sizeof(Vertex);
                    std::memcpy(current_offset, &vertex.tex_coord, sizeof(TextureCoord));
                    current_offset += sizeof(TextureCoord);

                    ++result.vertices_count;
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
        result.offset = 0;

        return result;
    }
}
