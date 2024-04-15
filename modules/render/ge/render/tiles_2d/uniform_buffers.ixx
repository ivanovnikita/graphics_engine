module;

#include <vector>

export module tiles_2d.uniform_buffers;

import vulkan_common.buffer_fwd;
import vulkan_common.device_fwd;

namespace ge::tiles
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
