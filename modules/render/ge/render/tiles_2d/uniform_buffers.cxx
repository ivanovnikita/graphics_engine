module;

#include <vulkan/vulkan.hpp>

module tiles_2d.uniform_buffers;

import view_proj_2d;
import vulkan_common.uniform_buffers;

namespace ge::tiles
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData& device_data,
        const size_t count
    )
    {
        return create_uniform_buffers(device_data, count, sizeof(ViewProj2d));
    }
}
