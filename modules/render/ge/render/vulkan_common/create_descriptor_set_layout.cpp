#include "create_descriptor_set_layout.h"
#include "exception.h"

namespace ge
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout
    (
        const vk::Device& device,
        const vk::DescriptorSetLayoutCreateInfo& create_info
    )
    {
        vk::DescriptorSetLayout layout;
        const vk::Result result = device.createDescriptorSetLayout(&create_info, nullptr, &layout);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Descriptor set layout creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Descriptor set layout creation failed");
        }
        }

        return vk::UniqueDescriptorSetLayout
        {
            std::move(layout),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
