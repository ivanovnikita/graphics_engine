module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module image_2d.pipeline_layout;

namespace ge::image2d
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
