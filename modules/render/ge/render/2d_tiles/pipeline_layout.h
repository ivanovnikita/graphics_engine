#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::tiles
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
