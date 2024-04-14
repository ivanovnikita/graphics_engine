module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.buffer;

export import vulkan_common.device;

namespace ge
{
    export struct BufferData final
    {
        static BufferData create_default
        (
            const DeviceData&,
            vk::BufferUsageFlags,
            vk::MemoryPropertyFlags,
            size_t size
        );

        vk::UniqueBuffer buffer;
        vk::UniqueDeviceMemory device_memory;
    };

    export vk::UniqueDeviceMemory allocate_device_memory
    (
        const DeviceData&,
        const vk::MemoryPropertyFlags,
        const vk::MemoryRequirements&
    );

    export void copy_buffer
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const vk::Buffer& src,
        const vk::Buffer& dst,
        const size_t size
    );

    export void* map_memory
    (
        const vk::Device&,
        const vk::DeviceMemory&,
        const vk::DeviceSize& offset,
        const vk::DeviceSize& size,
        const vk::MemoryMapFlags&
    );
}
