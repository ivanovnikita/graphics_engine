#include "vertex_buffer.h"

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

    std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> create_vertex_buffer
    (
        const vk::PhysicalDevice& physical_device
        , const vk::Device& logical_device
        , const size_t size
    )
    {
        const auto buffer_create_info = vk::BufferCreateInfo{}
            .setSize(size)
            .setUsage(vk::BufferUsageFlagBits::eVertexBuffer)
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
                    , vk::MemoryPropertyFlagBits::eHostVisible
                    | vk::MemoryPropertyFlagBits::eHostCoherent
                )
            );

        vk::UniqueDeviceMemory memory = logical_device.allocateMemoryUnique(alloc_info);

        logical_device.bindBufferMemory(*buffer, *memory, 0);

        return {std::move(buffer), std::move(memory)};
    }
}
