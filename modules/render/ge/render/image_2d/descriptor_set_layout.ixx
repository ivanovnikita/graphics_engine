module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module image_2d.descriptor_set_layout;

namespace ge::image2d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
