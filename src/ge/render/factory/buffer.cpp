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
}
