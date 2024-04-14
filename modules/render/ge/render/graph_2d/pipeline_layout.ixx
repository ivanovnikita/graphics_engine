module;

#include <vulkan/vulkan.hpp>

export module graph_2d.pipeline_layout;

namespace ge::graph
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
