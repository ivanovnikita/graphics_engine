#include "buffer.h"
#include "exception.h"

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

            const vk::MemoryAllocateInfo alloc_info
            {
                mem_requirements.size,
                find_memory_type_index
                (
                    device_data.physical_device_data,
                    mem_requirements.memoryTypeBits,
                    mem_properies
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
}
