module;

#include "ge/common/exception_macro.h"

#include <vulkan/vulkan.hpp>

module vulkan_common.uniform_buffers;

import exception;
import vulkan_common.buffer;
import vulkan_common.device;

namespace ge
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData& device_data,
        const size_t count,
        const size_t buffer_size
    )
    {
        std::vector<BufferData> buffers;

        try
        {
            buffers.reserve(count);
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for uniform buffers failed");
        }

        for (size_t i = 0; i < count; ++i)
        {
            buffers.emplace_back
            (
                BufferData::create_default
                (
                    device_data,
                    vk::BufferUsageFlagBits::eUniformBuffer,
                    vk::MemoryPropertyFlagBits::eHostVisible |
                    vk::MemoryPropertyFlagBits::eHostCoherent,
                    vk::DeviceSize{buffer_size}
                )
            );
        }

        return buffers;
    }
}
