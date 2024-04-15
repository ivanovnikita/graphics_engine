module;

#include "vulkan_fwds.h"

export module vulkan_common.create_descriptor_set_layout;

namespace ge
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout
    (
        const vk::Device&,
        const vk::DescriptorSetLayoutCreateInfo&
    );
}
