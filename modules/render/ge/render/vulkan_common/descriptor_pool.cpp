#include "descriptor_pool.h"
#include "exception.h"

namespace ge
{
    vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device& device,
        const size_t size,
        const std::span<const vk::DescriptorPoolSize> pool_sizes
    )
    {
        const vk::DescriptorPoolCreateInfo create_info = vk::DescriptorPoolCreateInfo()
            .setPoolSizeCount(static_cast<uint32_t>(pool_sizes.size()))
            .setPPoolSizes(pool_sizes.data())
            .setMaxSets(static_cast<uint32_t>(size))
            .setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);

        vk::DescriptorPool pool;
        const vk::Result result = device.createDescriptorPool(&create_info, nullptr, &pool);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorFragmentationEXT:
            GE_THROW_EXPECTED_RESULT(result, "Descriptor pool creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Descriptor pool creation failed");
        }
        }

        return vk::UniqueDescriptorPool
        {
            std::move(pool),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
