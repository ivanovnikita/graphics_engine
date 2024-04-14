module;

#include <vulkan/vulkan.hpp>

export module image_3d.descriptor_set_layout;

namespace ge::image3d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
