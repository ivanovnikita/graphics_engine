module;

#include "vulkan_fwds.h"

export module vulkan_common.create_pipeline;

namespace ge
{
    export vk::UniquePipeline create_pipeline
    (
        const vk::Device&,
        const vk::GraphicsPipelineCreateInfo&
    );
}
