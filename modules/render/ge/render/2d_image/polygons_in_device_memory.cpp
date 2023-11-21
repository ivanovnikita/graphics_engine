#include "polygons_in_device_memory.h"

#include "ge/common/safe_cast.hpp"

namespace ge::image2d
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
        static_assert(sizeof(TextureCoords) == 2 * sizeof(float));

        size_t triangle_points_memory_usage = 0;
        size_t texture_coords_memory_usage = 0;

        PolygonsInDeviceMemory result;
        result.vertices_count = 0;
        for (const Polygons& polygon : polygons)
        {
            triangle_points_memory_usage += 3 * sizeof(World2dCoords) * polygon.triangles.size();
            texture_coords_memory_usage += 3 * sizeof(TextureCoords) * polygon.triangles.size();
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
                for (const size_t ind : triangle.vertices)
                {
                    const Polygons::TexturedVertex& vertex = polygon.vertices[ind];

                    std::memcpy(current_offset, &vertex.coord, sizeof(World2dCoords));
                    current_offset += sizeof(World2dCoords);
                    std::memcpy(current_offset, &vertex.tex_coord, sizeof(TextureCoords));
                    current_offset += sizeof(TextureCoords);

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
