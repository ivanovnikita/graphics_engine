#include "ge/render/factory/descriptor_set_layout.h"

namespace ge::factory
{
    // The descriptor layout specifies the types of resources that are going to be accessed by the pipeline,
    // just like a render pass specifies the types of attachments that will be accessed.
    // A descriptor set specifies the actual buffer or image resources that will be bound to the descriptors,
    // just like a framebuffer specifies the actual image views to bind to render pass attachments.
    // The descriptor set is then bound for the drawing commands just like the vertex buffers and framebuffer.
    vk::UniqueDescriptorSetLayout camera_2d_descriptor_set_layout
    (
        const vk::Device& logical_device
    )
    {
        const auto binding = vk::DescriptorSetLayoutBinding{}
            .setBinding(0)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1) // more for arrays
            .setStageFlags(vk::ShaderStageFlagBits::eVertex); // at what stages it will be used

        const auto layout_create_info = vk::DescriptorSetLayoutCreateInfo{}
            .setBindingCount(1)
            .setPBindings(&binding);

        return logical_device.createDescriptorSetLayoutUnique(layout_create_info);
    }
}
