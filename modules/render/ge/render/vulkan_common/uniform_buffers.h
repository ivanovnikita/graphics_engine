#pragma once

#include "buffer.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace ge
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count,
        size_t buffer_size
    );
}
