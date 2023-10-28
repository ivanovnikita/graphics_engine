#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::PipelineLayoutCreateInfo&
    );
}
