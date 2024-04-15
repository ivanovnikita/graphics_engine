module;

#include <vector>

export module image_3d.uniform_buffers;

import vulkan_common.buffer_fwd;
import vulkan_common.device_fwd;

namespace ge::image3d
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
