module;

#include <vulkan/vulkan.hpp>

export module image_2d.descriptor_set_layout;

namespace ge::image2d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
