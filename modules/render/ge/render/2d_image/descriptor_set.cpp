#include "descriptor_set.h"
#include "ge/render/vulkan_common/descriptor_set.h"
#include "ge/render/camera/view_proj_2d.h"

namespace ge::image
{
    std::vector<vk::UniqueDescriptorSet> create_descriptor_sets
    (
        const vk::Device& device,
        const vk::DescriptorPool& pool,
        const vk::DescriptorSetLayout& layout,
        const size_t count,
        const std::span<const BufferData> uniform_buffers,
        const TextureImageData& texture_data
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
            const vk::DescriptorBufferInfo buffer_info = vk::DescriptorBufferInfo{}
                .setBuffer(*uniform_buffers[i].buffer)
                .setOffset(0)
                .setRange(sizeof(ViewProj2d));

            const vk::DescriptorImageInfo image_info = vk::DescriptorImageInfo{}
                .setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal)
                .setImageView(*texture_data.image_view)
                .setSampler(*texture_data.sampler);

            const std::array descriptor_writes
            {
                vk::WriteDescriptorSet{}
                    .setDstSet(*descriptor_sets[i])
                    .setDstBinding(0)
                    .setDstArrayElement(0)
                    .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                    .setDescriptorCount(1)
                    .setPBufferInfo(&buffer_info),
                vk::WriteDescriptorSet{}
                    .setDstSet(*descriptor_sets[i])
                    .setDstBinding(1)
                    .setDstArrayElement(0)
                    .setDescriptorType(vk::DescriptorType::eCombinedImageSampler)
                    .setDescriptorCount(1)
                    .setPImageInfo(&image_info),
            };

            device.updateDescriptorSets
            (
                static_cast<uint32_t>(descriptor_writes.size()),
                descriptor_writes.data(),
                0,
                nullptr
            );
        }

        return descriptor_sets;
    }
}

