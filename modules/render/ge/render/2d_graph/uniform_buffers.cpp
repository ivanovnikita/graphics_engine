#include "uniform_buffers.h"
#include "ge/render/view_proj_2d.h"
#include "ge/render/exception.h"

namespace ge::graph
{
    std::vector<BufferData> create_uniform_buffers
    (
        const DeviceData& device_data,
        const size_t count
    )
    {
        std::vector<BufferData> buffers;

        try
        {
            buffers.reserve(count);
        }
        catch (const std::bad_alloc& e)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for uniform buffers failed");
        }

        constexpr vk::DeviceSize buffer_size = sizeof(ViewProj2d);
        for (size_t i = 0; i < count; ++i)
        {
            buffers.emplace_back
            (
                BufferData::create_default
                (
                    device_data,
                    vk::BufferUsageFlagBits::eUniformBuffer,
                    vk::MemoryPropertyFlagBits::eHostVisible |
                    vk::MemoryPropertyFlagBits::eHostCoherent,
                    buffer_size
                )
            );
        }

        return buffers;
    }
}
