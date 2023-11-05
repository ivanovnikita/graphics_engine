#include "descriptor_set.h"
#include "ge/render/vulkan_common/descriptor_set.h"
#include "ge/render/camera/view_proj_2d.h"

namespace ge::tiles
{
    std::vector<vk::UniqueDescriptorSet> create_descriptor_sets
    (
        const vk::Device& device,
        const vk::DescriptorPool& pool,
        const vk::DescriptorSetLayout& layout,
        const size_t count,
        const std::span<const BufferData> uniform_buffers
    )
    {
        assert(uniform_buffers.size() == count);

        std::vector<vk::UniqueDescriptorSet> descriptor_sets = allocate_descriptor_sets
        (
            device,
            pool,
            layout,
            count
        );

        for (size_t i = 0; i < count; ++i)
        {
            const auto buffer_info = vk::DescriptorBufferInfo{}
                .setBuffer(*uniform_buffers[i].buffer)
                .setOffset(0)
                .setRange(sizeof(ViewProj2d));

            const auto descriptor_write = vk::WriteDescriptorSet{}
                .setDstSet(*descriptor_sets[i])
                .setDstBinding(0)
                .setDstArrayElement(0)
                .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                .setDescriptorCount(1)
                .setPBufferInfo(&buffer_info);

            device.updateDescriptorSets(1, &descriptor_write, 0, nullptr);
        }

        return descriptor_sets;
    }
}

