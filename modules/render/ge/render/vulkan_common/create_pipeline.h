#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniquePipeline create_pipeline
    (
        const vk::Device&,
        const vk::GraphicsPipelineCreateInfo&
    );
}
