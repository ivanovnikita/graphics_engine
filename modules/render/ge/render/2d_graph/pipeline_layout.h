#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
