#pragma once

#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::tiles
{
    std::vector<vk::UniqueDescriptorSet> create_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        const vk::DescriptorSetLayout&,
        size_t count,
        std::span<const BufferData> uniform_buffers
    );
}
