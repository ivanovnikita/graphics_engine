module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.pipeline_layout;

import vulkan_common.create_pipeline_layout;

namespace ge::tiles
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}

module : private;

namespace ge::tiles
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device& device,
        const vk::DescriptorSetLayout& descriptor_set_layout
    )
    {
        const auto layout_info = vk::PipelineLayoutCreateInfo()
            .setSetLayoutCount(1)
            .setPSetLayouts(&descriptor_set_layout)
            .setPushConstantRangeCount(0);

        return ge::create_pipeline_layout(device, layout_info);
    }
}