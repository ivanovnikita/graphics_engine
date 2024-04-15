module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module graph_2d.descriptor_set_layout;

namespace ge::graph
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
