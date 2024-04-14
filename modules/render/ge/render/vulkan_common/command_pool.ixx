module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

export module vulkan_common.command_pool;

export import vulkan_common.device;

import vulkan_common.exception;

namespace ge
{
    export vk::UniqueCommandPool create_command_pool(const DeviceData&);
}

module : private;

namespace ge
{
    vk::UniqueCommandPool create_command_pool(const DeviceData& device_data)
    {
        const auto pool_create_info = vk::CommandPoolCreateInfo()
            .setQueueFamilyIndex(device_data.graphics_queue_family_index);

        vk::CommandPool command_pool;
        const vk::Result result = device_data.logical_device->createCommandPool
        (
            &pool_create_info,
            nullptr,
            &command_pool
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Command pool creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Command pool creation failed");
        }
        }

        return vk::UniqueCommandPool
        {
            std::move(command_pool),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{*device_data.logical_device}
        };
    }
}
