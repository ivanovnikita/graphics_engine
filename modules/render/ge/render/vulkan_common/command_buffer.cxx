module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

module vulkan_common.command_buffer;

import vulkan_common.exception;
import vulkan_common.device;
import vulkan_common.fence;
import vulkan_common.queue;
import safe_cast;

namespace ge
{
    vk::UniqueCommandBuffer allocate_command_buffer
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool,
        const vk::CommandBufferLevel level
    )
    {
        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setLevel(level)
            .setCommandPool(command_pool)
            .setCommandBufferCount(1);

        vk::CommandBuffer command_buffer;
        const vk::Result result = device.allocateCommandBuffers
        (
            &alloc_info,
            &command_buffer
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Command buffer allocation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Command buffer allocation failed");
        }
        }

        vk::PoolFree
        <
            vk::Device,
            vk::CommandPool,
            VULKAN_HPP_DEFAULT_DISPATCHER_TYPE
        > deleter{device, command_pool};

        return vk::UniqueHandle<vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
        (
            std::move(command_buffer),
            deleter
        );
    }

    std::vector<vk::UniqueCommandBuffer> allocate_command_buffers
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool,
        const vk::CommandBufferLevel level,
        const size_t count
    )
    {
        std::vector<vk::CommandBuffer> buffers;
        try
        {
            buffers.resize(count);
        }
        catch(const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for command buffers failed");
        }

        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setLevel(level)
            .setCommandPool(command_pool)
            .setCommandBufferCount(safe_cast<uint32_t>(count));

        const vk::Result result = device.allocateCommandBuffers
        (
            &alloc_info,
            buffers.data()
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Command buffer allocation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Command buffer allocation failed");
        }
        }

        vk::PoolFree
        <
            vk::Device,
            vk::CommandPool,
            VULKAN_HPP_DEFAULT_DISPATCHER_TYPE
        > deleter{device, command_pool};

        std::vector<vk::UniqueCommandBuffer> unique_buffers;
        try
        {
            unique_buffers.reserve(count);
        }
        catch(const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for unique command buffers failed");
        }

        for (vk::CommandBuffer& buffer : buffers)
        {
            unique_buffers.emplace_back
            (
                vk::UniqueHandle<vk::CommandBuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>
                (
                    std::move(buffer),
                    deleter
                )
            );
        }

        return unique_buffers;
    }

    void begin
    (
        const vk::CommandBuffer& buffer,
        const vk::CommandBufferBeginInfo& info
    )
    {
        const vk::Result result = buffer.begin(&info);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Command buffer begin failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Command buffer begin failed");
        }
        }
    }

    void end(const vk::CommandBuffer& buffer)
    {
        const vk::Result result = static_cast<vk::Result>(vkEndCommandBuffer(buffer));

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Command buffer end failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Command buffer end failed");
        }
        }
    }

    vk::UniqueCommandBuffer create_one_time_commands
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool
    )
    {
        vk::UniqueCommandBuffer command_buffer = allocate_command_buffer
        (
            device,
            command_pool,
            vk::CommandBufferLevel::ePrimary
        );

        const auto begin_info = vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        begin(*command_buffer, begin_info);

        return command_buffer;
    }

    void submit_one_time_commands
    (
        vk::UniqueCommandBuffer command_buffer,
        const DeviceData& device_data,
        const vk::Fence& fence
    )
    {
        end(*command_buffer);

        const std::span fences{&fence, 1};
        reset_fences(*device_data.logical_device, fences);

        const auto submit_info = vk::SubmitInfo{}
            .setCommandBufferCount(1)
            .setPCommandBuffers(&*command_buffer);

        submit(device_data.graphics_queue, {&submit_info, 1}, fence);

        wait_for_fences(*device_data.logical_device, fences, true, std::chrono::nanoseconds::max());
    }
}
