#include "polygons_in_device_memory.h"

#include "ge/common/safe_cast.hpp"

namespace ge::image3d
{
    PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>& polygons
    )
    {
        static_assert(sizeof(World3dCoords) == 3 * sizeof(float));
        static_assert(sizeof(TextureCoords) == 2 * sizeof(float));

        size_t vertex_buffer_memory_usage = 0;
        size_t index_buffer_memory_usage = 0;

        PolygonsInDeviceMemory result;
        result.vertices_count = 0;
        result.indices_count = 0;
        for (const Polygons& polygon : polygons)
        {
            vertex_buffer_memory_usage += (sizeof(World3dCoords) + sizeof(TextureCoords)) * polygon.vertices.size();
            index_buffer_memory_usage += 3 * sizeof(uint32_t) * polygon.triangles.size();
        }

        const vk::DeviceSize vertex_buffer_size = safe_cast<vk::DeviceSize>(vertex_buffer_memory_usage);

        BufferData vertex_staging_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent,
            vertex_buffer_size
        );

        constexpr vk::DeviceSize offset = 0;
        void* const vertex_memory_start = map_memory
        (
            *device_data.logical_device,
            *vertex_staging_buffer.device_memory,
            offset,
            vertex_buffer_size,
            vk::MemoryMapFlags{}
        );

        uint8_t* current_offset = static_cast<uint8_t*>(vertex_memory_start);

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::TexturedVertex& vertex : polygon.vertices)
            {
                std::memcpy(current_offset, &vertex.world_coords, sizeof(World3dCoords));
                current_offset += sizeof(World3dCoords);
                std::memcpy(current_offset, &vertex.texture_coords, sizeof(TextureCoords));
                current_offset += sizeof(TextureCoords);

                ++result.vertices_count;
            }
        }

        assert(current_offset == static_cast<uint8_t*>(vertex_memory_start) + vertex_buffer_size);

        device_data.logical_device->unmapMemory(*vertex_staging_buffer.device_memory);

        BufferData vertex_device_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            vertex_buffer_size
        );

        copy_buffer
        (
            device_data
            , command_pool
            , transfer_finished
            , *vertex_staging_buffer.buffer
            , *vertex_device_buffer.buffer
            , vertex_buffer_size
        );

        result.vertex_buffer = std::move(vertex_device_buffer);

        const vk::DeviceSize index_buffer_size = safe_cast<vk::DeviceSize>(index_buffer_memory_usage);

        BufferData index_staging_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferSrc,
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent,
            index_buffer_size
        );

        void* const index_memory_start = map_memory
        (
            *device_data.logical_device,
            *index_staging_buffer.device_memory,
            offset,
            index_buffer_size,
            vk::MemoryMapFlags{}
        );

        current_offset = static_cast<uint8_t*>(index_memory_start);

        for (const Polygons& polygon : polygons)
        {
            for (const Polygons::Triangle& triangle : polygon.triangles)
            {
                for (const size_t ind : triangle.inds)
                {
                    const uint32_t i = static_cast<uint32_t>(ind);
                    std::memcpy(current_offset, &i, sizeof(uint32_t));
                    current_offset += sizeof(uint32_t);

                    ++result.indices_count;
                }
            }
        }

        assert(current_offset == static_cast<uint8_t*>(index_memory_start) + index_buffer_size);

        device_data.logical_device->unmapMemory(*index_staging_buffer.device_memory);

        BufferData index_device_buffer = BufferData::create_default
        (
            device_data,
            vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
            vk::MemoryPropertyFlagBits::eDeviceLocal,
            index_buffer_size
        );

        copy_buffer
        (
            device_data
            , command_pool
            , transfer_finished
            , *index_staging_buffer.buffer
            , *index_device_buffer.buffer
            , index_buffer_size
        );

        result.index_buffer = std::move(index_device_buffer);

        return result;
    }
}
