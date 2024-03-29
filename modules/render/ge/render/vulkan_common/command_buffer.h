#pragma once

#include "device.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace ge
{
    vk::UniqueCommandBuffer allocate_command_buffer
    (
        const vk::Device&,
        const vk::CommandPool&,
        vk::CommandBufferLevel
    );

    std::vector<vk::UniqueCommandBuffer> allocate_command_buffers
    (
        const vk::Device&,
        const vk::CommandPool&,
        vk::CommandBufferLevel,
        size_t count
    );

    void begin(const vk::CommandBuffer&, const vk::CommandBufferBeginInfo&);
    void end(const vk::CommandBuffer&);

    vk::UniqueCommandBuffer create_one_time_commands
    (
        const vk::Device&,
        const vk::CommandPool&
    );

    void submit_one_time_commands
    (
        vk::UniqueCommandBuffer,
        const DeviceData&,
        const vk::Fence&
    );
}
