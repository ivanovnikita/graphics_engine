#include "ge/render/factory/descriptor_pool.h"

namespace ge::factory
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device& logical_device
      , const size_t size
    )
    {
        const auto pool_size = vk::DescriptorPoolSize{}
            .setType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(static_cast<uint32_t>(size));


        const auto create_info = vk::DescriptorPoolCreateInfo{}
            .setPoolSizeCount(1)
            .setPPoolSizes(&pool_size)
            .setMaxSets(static_cast<uint32_t>(size));

        return logical_device.createDescriptorPoolUnique(create_info);
    }
}
