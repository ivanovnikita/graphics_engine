#pragma once

#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace ge::image3d
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
