#pragma once

#include "ge/render/vulkan_common/buffer.h"
#include "ge/render/vulkan_common/texture_image.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge::image
{
    std::vector<vk::UniqueDescriptorSet> create_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        const vk::DescriptorSetLayout&,
        size_t count,
        std::span<const BufferData> uniform_buffers,
        const TextureImageData&
    );
}
