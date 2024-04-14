module;

#include <vulkan/vulkan.hpp>

export module image_2d.pipeline_layout;

namespace ge::image2d
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
