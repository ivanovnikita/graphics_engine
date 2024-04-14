module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.create_pipeline;

namespace ge
{
    export vk::UniquePipeline create_pipeline
    (
        const vk::Device&,
        const vk::GraphicsPipelineCreateInfo&
    );
}
