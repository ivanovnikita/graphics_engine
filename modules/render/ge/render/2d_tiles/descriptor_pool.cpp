#include "descriptor_pool.h"
#include "ge/render/vulkan_common/descriptor_pool.h"

namespace ge::tiles
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device& device,
        const size_t size
    )
    {
        const vk::DescriptorPoolSize pool_size
        {
            vk::DescriptorType::eUniformBuffer,
            static_cast<uint32_t>(size)
        };

        return ge::create_descriptor_pool(device, size, {&pool_size, 1});
    }
}
