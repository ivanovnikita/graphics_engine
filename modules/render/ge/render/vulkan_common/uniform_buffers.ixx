module;

#include <vector>

export module vulkan_common.uniform_buffers;

import vulkan_common.buffer_fwd;
import vulkan_common.device_fwd;

namespace ge
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count,
        size_t buffer_size
    );
}
