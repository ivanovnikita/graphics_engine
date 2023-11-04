#include "descriptor_pool.h"
#include "ge/render/vulkan_common/descriptor_pool.h"

namespace ge::image
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device& device,
        const size_t size
    )
    {
        const std::array pool_sizes
        {
            vk::DescriptorPoolSize
            {
                vk::DescriptorType::eUniformBuffer,
                static_cast<uint32_t>(size)
            },
            vk::DescriptorPoolSize
            {
                vk::DescriptorType::eCombinedImageSampler,
                static_cast<uint32_t>(size)
            },
        };

        return ge::create_descriptor_pool(device, size, pool_sizes);
    }
}
