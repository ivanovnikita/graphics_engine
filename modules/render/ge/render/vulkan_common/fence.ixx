module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <chrono>
#include <span>

export module vulkan_common.fence;

import vulkan_common.exception;
import safe_cast;

namespace ge
{
    export vk::UniqueFence create_fence(const vk::Device&);
    export void reset_fences(const vk::Device&, std::span<const vk::Fence>);

    // TODO: return enum Success/Timeout
    export void wait_for_fences
    (
        const vk::Device&,
        std::span<const vk::Fence>,
        bool wait_all,
        std::chrono::nanoseconds timeout
    );
}

module : private;

namespace ge
{
    vk::UniqueFence create_fence(const vk::Device& device)
    {
        const vk::FenceCreateInfo create_info;
        vk::Fence fence;
        const vk::Result result = device.createFence
        (
            &create_info,
            nullptr,
            &fence
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Fence creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Fence creation failed");
        }
        }

        return vk::UniqueFence
        {
            std::move(fence),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }

    void reset_fences(const vk::Device& device, const std::span<const vk::Fence> fences)
    {
        const vk::Result result = static_cast<vk::Result>
        (
            vkResetFences
            (
                device,
                static_cast<uint32_t>(fences.size()),
                reinterpret_cast<const VkFence*>(fences.data())
            )
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Fences reseting failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Fences reseting failed");
        }
        }
    }

    void wait_for_fences
    (
        const vk::Device& device,
        const std::span<const vk::Fence> fences,
        const bool wait_all,
        const std::chrono::nanoseconds timeout
    )
    {
        const vk::Result result = static_cast<vk::Result>
        (
            vkWaitForFences
            (
                device,
                static_cast<uint32_t>(fences.size()),
                reinterpret_cast<const VkFence*>(fences.data()),
                wait_all,
                safe_cast<uint64_t>(timeout.count())
            )
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eTimeout:
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
            GE_THROW_EXPECTED_RESULT(result, "Fences waiting failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Fences waiting failed");
        }
        }
    }
}
