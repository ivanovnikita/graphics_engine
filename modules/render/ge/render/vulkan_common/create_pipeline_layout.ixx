module;

#include "vulkan_fwds.h"

export module vulkan_common.create_pipeline_layout;

namespace ge
{
    export vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::PipelineLayoutCreateInfo&
    );
}
