#include "ge/render/factory/descriptor_set_layout.h"

namespace ge::factory
{
    vk::UniqueDescriptorSetLayout create_descriptor_set_layout
    (
        const vk::Device& logical_device
    )
    {
        const auto binding = vk::DescriptorSetLayoutBinding{}
            .setBinding(0)
            .setDescriptorType(vk::DescriptorType::eUniformBuffer)
            .setDescriptorCount(1)
            .setStageFlags(vk::ShaderStageFlagBits::eVertex);

        const auto layout_create_info = vk::DescriptorSetLayoutCreateInfo{}
            .setBindingCount(1)
            .setPBindings(&binding);

        return logical_device.createDescriptorSetLayoutUnique(layout_create_info);
    }
}
