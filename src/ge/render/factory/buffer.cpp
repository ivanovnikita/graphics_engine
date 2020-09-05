#include "buffer.hpp"

namespace ge::factory
{
    namespace
    {
        uint32_t find_memory_type
        (
            const vk::PhysicalDevice& physical_device
            , const uint32_t type_filter
            , const vk::MemoryPropertyFlags properties
        )
        {
            const vk::PhysicalDeviceMemoryProperties mem_properties = physical_device.getMemoryProperties();
            for (uint32_t i = 0; i < mem_properties.memoryTypeCount; ++i)
            {
                if
                (
                    (type_filter & (1u << i))
                    and ((mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
                )
                {
                    return i;
                }
            }

            throw std::runtime_error("Failed to find suitable memory type");
        }        
    }

    // TODO: report allocation failure
    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> create_buffer
    (
        const vk::PhysicalDevice& physical_device
        , const vk::Device& logical_device
        , const size_t size
        , const vk::BufferUsageFlags usage
        , const vk::MemoryPropertyFlags mem_properties
    )
    {
        const auto buffer_create_info = vk::BufferCreateInfo{}
            .setSize(size)
            .setUsage(usage)
            .setSharingMode(vk::SharingMode::eExclusive);

        vk::UniqueBuffer buffer = logical_device.createBufferUnique(buffer_create_info);
        const vk::MemoryRequirements mem_requirements = logical_device.getBufferMemoryRequirements(*buffer);

        const auto alloc_info = vk::MemoryAllocateInfo{}
            .setAllocationSize(mem_requirements.size)
            .setMemoryTypeIndex
            (
                find_memory_type
                (
                    physical_device
                    , mem_requirements.memoryTypeBits
                    , mem_properties
                )
            );

        vk::UniqueDeviceMemory memory = logical_device.allocateMemoryUnique(alloc_info);

        logical_device.bindBufferMemory(*buffer, *memory, 0);

        return {std::move(buffer), std::move(memory)};
    }

    void copy_buffer
    (
        const vk::Device& logical_device
        , const vk::CommandPool& command_pool
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const vk::Buffer& src
        , const vk::Buffer& dst
        , const size_t size
    )
    {
        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandPool(command_pool)
            .setCommandBufferCount(1);

        const std::vector<vk::UniqueCommandBuffer> command_buffers = logical_device.allocateCommandBuffersUnique
        (
            alloc_info
        );
        const vk::CommandBuffer& command_buffer = *command_buffers.front();

        const auto begin_info = vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        command_buffer.begin(begin_info);
        const auto copy_region = vk::BufferCopy{}
            .setSrcOffset(0)
            .setDstOffset(0)
            .setSize(size);
        command_buffer.copyBuffer(src, dst, copy_region);
        command_buffer.end();

        const auto submit_info = vk::SubmitInfo{}
            .setCommandBufferCount(1)
            .setPCommandBuffers(&command_buffer);

        logical_device.resetFences(1, &transfer_finished);
        transfer.submit(1, &submit_info, transfer_finished);

        constexpr uint64_t timeout = std::numeric_limits<uint64_t>::max();
        logical_device.waitForFences(1, &transfer_finished, VK_TRUE, timeout);
    }

    GraphInDeviceMemory load_graph_to_device
    (
        const vk::PhysicalDevice& physical_device
        , const vk::Device& logical_device
        , const vk::CommandPool& command_pool
        , const vk::Queue& transfer
        , const vk::Fence& transfer_finished
        , const Graph& graph
    )
    {
        static_assert(sizeof(Vertex) == 2 * sizeof(float));
        static_assert(sizeof(Color) == 3 * sizeof(float));

        const size_t arc_points_memory_usage = sizeof(Vertex) * graph.arcs.size() * 2;
        const size_t vertice_points_memory_usage = sizeof(Vertex) * graph.vertices.size();
        const size_t arc_colors_memory_usage = sizeof(Color) * graph.arcs.size() * 2;
        const size_t vertice_colors_memory_usage = sizeof(Color) * graph.vertices.size();

        const vk::DeviceSize buffer_size = safe_cast<vk::DeviceSize>
        (
            arc_points_memory_usage
            + vertice_points_memory_usage
            + arc_colors_memory_usage
            + vertice_colors_memory_usage
        );

        auto [staging_buffer, staging_memory] = factory::create_buffer
        (
            physical_device
            , logical_device
            , buffer_size
            , vk::BufferUsageFlagBits::eTransferSrc
            , vk::MemoryPropertyFlagBits::eHostVisible
            | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        constexpr vk::DeviceSize offset = 0;
        [[maybe_unused]] void* memory_start = logical_device.mapMemory
        (
            *staging_memory
            , offset
            , buffer_size
            , vk::MemoryMapFlags{}
        );

        uint8_t* current_offset = static_cast<uint8_t*>(memory_start);

        for (const Graph::Arc& arc : graph.arcs)
        {
            std::memcpy(current_offset, &graph.points[arc.index_from], sizeof(Vertex));
            current_offset += sizeof(Vertex);

            std::memcpy(current_offset, &graph.points[arc.index_to], sizeof(Vertex));
            current_offset += sizeof(Vertex);
        }

        for (const Graph::Vertice& vertice : graph.vertices)
        {
            std::memcpy(current_offset, &graph.points[vertice.index], sizeof(Vertex));
            current_offset += sizeof(Vertex);
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

        logical_device.unmapMemory(*staging_memory);

        auto [buffer, memory] = factory::create_buffer
        (
            physical_device
          , logical_device
          , buffer_size
          , vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer
          , vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        copy_buffer
        (
            logical_device
            , command_pool
            , transfer
            , transfer_finished
            , *staging_buffer
            , *buffer
            , buffer_size
        );

        GraphInDeviceMemory result;
        result.memory = std::move(memory);
        result.buffer = std::move(buffer);
        result.arc_points_offset = 0;
        result.vertice_points_offset = result.arc_points_offset + arc_points_memory_usage;
        result.arc_colors_offset = result.vertice_points_offset + vertice_points_memory_usage;
        result.vertice_colors_offset = result.arc_colors_offset + arc_colors_memory_usage;
        result.arc_points_count = graph.arcs.size() * 2;
        result.vertice_points_count = graph.vertices.size();

        return result;
    }

    PolygonsInDeviceMemory load_polygons_to_device
    (
        const vk::PhysicalDevice& physical_device,
        const vk::Device& logical_device,
        const vk::CommandPool& command_pool,
        const vk::Queue& transfer,
        const vk::Fence& transfer_finished,
        const Polygons& polygons
    )
    {
        static_assert(sizeof(Vertex) == 2 * sizeof(float));
        static_assert(sizeof(Color) == 3 * sizeof(float));

        const size_t vertice_points_memory_usage = 3 * sizeof(Vertex) * polygons.triangles.size();
        const size_t vertice_colors_memory_usage = 3 * sizeof(Color) * polygons.triangles.size();

        const vk::DeviceSize buffer_size = safe_cast<vk::DeviceSize>
        (
            vertice_points_memory_usage + vertice_colors_memory_usage
        );

        auto [staging_buffer, staging_memory] = factory::create_buffer
        (
            physical_device
            , logical_device
            , buffer_size
            , vk::BufferUsageFlagBits::eTransferSrc
            , vk::MemoryPropertyFlagBits::eHostVisible
            | vk::MemoryPropertyFlagBits::eHostCoherent
        );

        constexpr vk::DeviceSize offset = 0;
        [[maybe_unused]] void* memory_start = logical_device.mapMemory
        (
            *staging_memory
            , offset
            , buffer_size
            , vk::MemoryMapFlags{}
        );

        uint8_t* current_offset = static_cast<uint8_t*>(memory_start);

        for (const Polygons::Triangle& triangle : polygons.triangles)
        {
            for (const size_t ind : triangle.inds)
            {
                std::memcpy(current_offset, &polygons.points[ind], sizeof(Vertex));
                current_offset += sizeof(Vertex);
            }
        }

        for (const Polygons::Triangle& triangle : polygons.triangles)
        {
            for (size_t i = 0; i < 3; ++i)
            {
                std::memcpy(current_offset, &triangle.color, sizeof(Color));
                current_offset += sizeof(Color);
            }
        }

        assert(current_offset == static_cast<uint8_t*>(memory_start) + buffer_size);

        logical_device.unmapMemory(*staging_memory);

        auto [buffer, memory] = factory::create_buffer
        (
            physical_device
          , logical_device
          , buffer_size
          , vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer
          , vk::MemoryPropertyFlagBits::eDeviceLocal
        );

        copy_buffer
        (
            logical_device
            , command_pool
            , transfer
            , transfer_finished
            , *staging_buffer
            , *buffer
            , buffer_size
        );

        PolygonsInDeviceMemory result;
        result.memory = std::move(memory);
        result.buffer = std::move(buffer);
        result.vertice_points_offset = 0;
        result.vertice_colors_offset = result.vertice_points_offset + vertice_points_memory_usage;
        result.vertice_points_count = 3 * polygons.triangles.size();

        return result;
    }
}
