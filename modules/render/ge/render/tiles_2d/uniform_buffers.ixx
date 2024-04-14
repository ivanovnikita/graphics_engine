module;

#include <vulkan/vulkan.hpp>

#include <vector>

export module tiles_2d.uniform_buffers;

export import vulkan_common.buffer;

namespace ge::tiles
{
    export std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData&,
        size_t count
    );
}
