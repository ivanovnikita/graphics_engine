#pragma once

#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace ge::image2d
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
