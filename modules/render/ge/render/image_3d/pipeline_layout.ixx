module;

#include <vulkan/vulkan.hpp>

export module image_3d.pipeline_layout;

namespace ge::image3d
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
