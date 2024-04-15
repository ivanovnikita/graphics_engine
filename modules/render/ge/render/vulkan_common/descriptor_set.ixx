module;

#include "vulkan_fwds.h"

#include <span>
#include <vector>

export module vulkan_common.descriptor_set;

namespace ge
{
    export std::vector<vk::UniqueDescriptorSet> allocate_descriptor_sets
    (
        const vk::Device&,
        const vk::DescriptorPool&,
        const vk::DescriptorSetLayout&,
        size_t count
    );
}
