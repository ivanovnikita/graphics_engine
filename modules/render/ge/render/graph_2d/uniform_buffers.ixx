module;

#include <vector>

export module graph_2d.uniform_buffers;

import vulkan_common.buffer;
import vulkan_common.device;

namespace ge::graph
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
