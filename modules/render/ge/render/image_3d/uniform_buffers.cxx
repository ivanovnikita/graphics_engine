module;

#include <vulkan/vulkan.hpp>

module image_3d.uniform_buffers;

import mvp_3d;
import vulkan_common.uniform_buffers;

namespace ge::image3d
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData& device_data,
        const size_t count
    )
    {
        return create_uniform_buffers(device_data, count, sizeof(Mvp3d));
    }
}
