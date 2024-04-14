module;

#include <vulkan/vulkan.hpp>

module image_2d.descriptor_pool;

import vulkan_common.descriptor_pool;

namespace ge::image2d
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
