module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

#include <span>

export module graph_2d.descriptor_set;

export import vulkan_common.buffer;

namespace ge::graph
{
    export std::vector<vk::UniqueDescriptorSet> create_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        const vk::DescriptorSetLayout&,
        size_t count,
        std::span<const BufferData> uniform_buffers
    );
}
