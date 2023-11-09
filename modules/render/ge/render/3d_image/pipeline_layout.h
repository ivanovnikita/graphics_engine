#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayout&
    );
}
