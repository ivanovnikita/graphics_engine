module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.pipeline_layout;

namespace ge::tiles
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
