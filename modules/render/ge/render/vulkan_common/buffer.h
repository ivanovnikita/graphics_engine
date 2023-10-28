#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct BufferData final
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

    void copy_buffer
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const vk::Buffer& src,
        const vk::Buffer& dst,
        const size_t size
    );

    void* map_memory
    (
        const vk::Device&,
        const vk::DeviceMemory&,
        const vk::DeviceSize& offset,
        const vk::DeviceSize& size,
        const vk::MemoryMapFlags&
    );
}
