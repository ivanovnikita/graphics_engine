module;

#include <vulkan/vulkan.hpp>

export module image_3d.descriptor_pool;

import vulkan_common.descriptor_pool;

namespace ge::image3d
{
    export vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}

module : private;

namespace ge::image3d
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
