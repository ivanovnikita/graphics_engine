#pragma once

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

namespace ge
{
    std::vector<vk::UniqueDescriptorSet> allocate_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        std::span<const vk::DescriptorSetLayout>
    );
}
