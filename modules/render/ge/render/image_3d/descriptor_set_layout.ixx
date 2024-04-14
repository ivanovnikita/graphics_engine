module;

#include <vulkan/vulkan.hpp>

export module image_3d.descriptor_set_layout;

import vulkan_common.create_descriptor_set_layout;

namespace ge::image3d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}

module : private;

namespace ge::image3d
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device& device)
    {
        const std::array bindings
        {
            vk::DescriptorSetLayoutBinding{}
                .setBinding(0)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(1)
                .setStageFlags(vk::ShaderStageFlagBits::eVertex),
            vk::DescriptorSetLayoutBinding{}
                .setBinding(1)
                .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                .setDescriptorCount(1)
                .setStageFlags(vk::ShaderStageFlagBits::eFragment)
        };

        const auto layout_create_info = vk::DescriptorSetLayoutCreateInfo{}
            .setBindingCount(static_cast<uint32_t>(bindings.size()))
            .setPBindings(bindings.data());

        return ge::create_descriptor_set_layout(device, layout_create_info);
    }
}
