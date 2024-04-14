module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.descriptor_set_layout;

namespace ge::tiles
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
