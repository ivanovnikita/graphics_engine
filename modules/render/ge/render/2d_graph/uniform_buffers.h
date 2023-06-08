#pragma once

#include "ge/render/buffer.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace ge::graph
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
