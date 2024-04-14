module;

#include <vulkan/vulkan.hpp>

#include <vector>

export module image_3d.uniform_buffers;

export import vulkan_common.buffer;

import mvp_3d;
import vulkan_common.uniform_buffers;

namespace ge::image3d
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}

module : private;

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
