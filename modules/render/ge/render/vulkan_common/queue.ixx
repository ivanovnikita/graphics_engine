module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <optional>

#include <span>

export module vulkan_common.queue;

import vulkan_common.exception;

namespace ge
{
    export struct QueueFamilyIndices final
    {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> compute;
        std::optional<uint32_t> transfer;
    };

    export struct Queues final
    {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue compute;
        vk::Queue transfer;
    };

    export void submit
    (
        const vk::Queue&,
        std::span<const vk::SubmitInfo>,
        const vk::Fence&
    );
}

module : private;

namespace ge
{
    void submit
    (
        const vk::Queue& queue,
        const std::span<const vk::SubmitInfo> submit_infos,
        const vk::Fence& fence
    )
    {
        const vk::Result result = queue.submit
        (
            static_cast<uint32_t>(submit_infos.size()),
            submit_infos.data(),
            fence
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorDeviceLost:
            GE_THROW_EXPECTED_RESULT(result, "Queue submit failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Queue submit failed");
        }
        }
    }
}
