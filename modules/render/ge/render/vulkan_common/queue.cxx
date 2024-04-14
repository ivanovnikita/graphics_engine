module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

module vulkan_common.queue;

import vulkan_common.exception;

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
