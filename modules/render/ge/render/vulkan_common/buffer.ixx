module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <limits>

export module vulkan_common.buffer;

export import vulkan_common.device;

import vulkan_common.exception;
import vulkan_common.command_buffer;
import vulkan_common.fence;
import vulkan_common.queue;


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

module : private;

namespace ge
{
    namespace
    {
        vk::UniqueBuffer create_buffer
        (
            const vk::Device& device,
            const vk::BufferUsageFlags usage,
            const size_t size
        )
        {
            const vk::BufferCreateInfo create_info = vk::BufferCreateInfo{}
                .setSize(size)
                .setUsage(usage)
                .setSharingMode(vk::SharingMode::eExclusive);

            vk::Buffer buffer;
            const vk::Result result = device.createBuffer
            (
                &create_info,
                nullptr,
                &buffer
            );
            switch (result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
                GE_THROW_EXPECTED_RESULT(result, "Buffer creation failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result, "Buffer creation failed");
            }
            }

            return vk::UniqueBuffer
            {
                std::move(buffer),
                vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
            };
        }

        uint32_t find_memory_type_index
        (
            const PhysicalDeviceData& physical_device_data,
            const uint32_t type_filter,
            const vk::MemoryPropertyFlags properties
        )
        {
            const vk::PhysicalDeviceMemoryProperties& mem_properties = physical_device_data.memory;
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

            GE_THROW_EXPECTED_ERROR("Failed to find suitable memory type for buffer");
        }

        vk::UniqueDeviceMemory allocate_device_memory
        (
            const DeviceData& device_data,
            const vk::Buffer& buffer,
            const vk::MemoryPropertyFlags mem_properies
        )
        {
            const vk::Device& device = *device_data.logical_device;
            const vk::MemoryRequirements mem_requirements = device.getBufferMemoryRequirements(buffer);

            return allocate_device_memory(device_data, mem_properies, mem_requirements);
        }
    }

    BufferData BufferData::create_default
    (
        const DeviceData& device_data,
        const vk::BufferUsageFlags usage,
        const vk::MemoryPropertyFlags mem_properies,
        const size_t size
    )
    {
        vk::UniqueBuffer buffer = create_buffer(*device_data.logical_device, usage, size);
        vk::UniqueDeviceMemory memory = allocate_device_memory(device_data, *buffer, mem_properies);

        const vk::Result result = vk::Result
        {
            vkBindBufferMemory
            (
                *device_data.logical_device,
                *buffer,
                *memory,
                0
            )
        };
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
            GE_THROW_EXPECTED_RESULT(result, "Binding device memory to buffer failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Binding device memory to buffer failed");
        }
        }

        return BufferData
        {
            .buffer = std::move(buffer),
            .device_memory = std::move(memory)
        };
    }

    vk::UniqueDeviceMemory allocate_device_memory
    (
        const DeviceData& device_data,
        const vk::MemoryPropertyFlags mem_properties,
        const vk::MemoryRequirements& mem_requirements
    )
    {
        const vk::Device& device = *device_data.logical_device;
        const vk::MemoryAllocateInfo alloc_info
        {
            mem_requirements.size,
            find_memory_type_index
            (
                device_data.physical_device_data,
                mem_requirements.memoryTypeBits,
                mem_properties
            )
        };

        vk::DeviceMemory memory;
        const vk::Result result = device.allocateMemory(&alloc_info, nullptr, &memory);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidExternalHandle:
        case vk::Result::eErrorInvalidOpaqueCaptureAddressKHR:
            GE_THROW_EXPECTED_RESULT(result, "Device memory allocation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Device memory allocation failed");
        }
        }

        return vk::UniqueDeviceMemory
        {
            std::move(memory),
            vk::ObjectFree<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }

    void copy_buffer
    (
        const DeviceData& device_data,
        const vk::CommandPool& command_pool,
        const vk::Fence& transfer_finished,
        const vk::Buffer& src,
        const vk::Buffer& dst,
        const size_t size
    )
    {
        vk::UniqueCommandBuffer command_buffer = create_one_time_commands
        (
            *device_data.logical_device,
            command_pool
        );

        const auto copy_region = vk::BufferCopy{}
            .setSrcOffset(0)
            .setDstOffset(0)
            .setSize(size);
        command_buffer->copyBuffer(src, dst, copy_region);

        submit_one_time_commands(std::move(command_buffer), device_data, transfer_finished);
    }

    void* map_memory
    (
        const vk::Device& device,
        const vk::DeviceMemory& memory,
        const vk::DeviceSize& offset,
        const vk::DeviceSize& size,
        const vk::MemoryMapFlags& flags
    )
    {
        void* memory_start = nullptr;
        const vk::Result map_result = device.mapMemory
        (
            memory,
            offset,
            size,
            flags,
            &memory_start
        );
        switch (map_result)
        {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorMemoryMapFailed:
                GE_THROW_EXPECTED_RESULT(map_result, "Memory mapping failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(map_result, "Memory mapping failed");
            }
        }

        return memory_start;
    }
}
