#include "uniform_buffers.h"
#include "ge/render/camera/mvp_3d.h"
#include "ge/render/vulkan_common/uniform_buffers.h"

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
