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
}
