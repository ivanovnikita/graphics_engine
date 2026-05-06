module;

#include <vector>

export module image_2d.uniform_buffers;

import vulkan_common.buffer;
import vulkan_common.device;

namespace ge::image2d
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
