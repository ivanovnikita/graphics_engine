#include "pipeline_layout.h"
#include "ge/render/vulkan_common/create_pipeline_layout.h"

namespace ge::image3d
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
