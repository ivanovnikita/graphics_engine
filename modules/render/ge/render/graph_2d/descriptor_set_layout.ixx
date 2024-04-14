module;

#include <vulkan/vulkan.hpp>

export module graph_2d.descriptor_set_layout;

namespace ge::graph
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
