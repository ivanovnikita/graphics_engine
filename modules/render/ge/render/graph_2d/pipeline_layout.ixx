module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module graph_2d.pipeline_layout;

namespace ge::graph
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
