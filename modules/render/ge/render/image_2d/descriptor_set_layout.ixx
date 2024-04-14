module;

#include <vulkan/vulkan.hpp>

export module image_2d.descriptor_set_layout;

import vulkan_common.create_descriptor_set_layout;

namespace ge::image2d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}

module : private;

namespace ge::image2d
{
    // The descriptor layout specifies the types of resources that are going to be accessed by the pipeline,
    // just like a render pass specifies the types of attachments that will be accessed.
    // A descriptor set specifies the actual buffer or image resources that will be bound to the descriptors,
    // just like a framebuffer specifies the actual image views to bind to render pass attachments.
    // The descriptor set is then bound for the drawing commands just like the vertex buffers and framebuffer.
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device& device)
    {
        const std::array bindings
        {
            vk::DescriptorSetLayoutBinding{}
                .setBinding(0)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer) // for passing 2d camera through uniform variable
                .setDescriptorCount(1) // more for arrays
                .setStageFlags(vk::ShaderStageFlagBits::eVertex), // at what stages it will be used
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
