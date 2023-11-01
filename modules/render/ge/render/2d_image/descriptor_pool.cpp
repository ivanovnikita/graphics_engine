#include "descriptor_pool.h"
#include "ge/render/vulkan_common/exception.h"

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
