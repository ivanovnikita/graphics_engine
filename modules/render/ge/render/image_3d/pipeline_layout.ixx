module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module image_3d.pipeline_layout;

namespace ge::image3d
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
