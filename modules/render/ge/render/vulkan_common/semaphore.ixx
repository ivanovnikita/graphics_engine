module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

export module vulkan_common.semaphore;

import vulkan_common.exception;

namespace ge
{
    export vk::UniqueSemaphore create_semaphore(const vk::Device&);
}

module : private;

namespace ge
{
    vk::UniqueSemaphore create_semaphore(const vk::Device& device)
    {
        const vk::SemaphoreCreateInfo create_info;
        vk::Semaphore semaphore;
        const vk::Result result = device.createSemaphore
        (
            &create_info,
            nullptr,
            &semaphore
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Semaphore creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Semaphore creation failed");
        }
        }

        return vk::UniqueSemaphore
        {
            std::move(semaphore),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
