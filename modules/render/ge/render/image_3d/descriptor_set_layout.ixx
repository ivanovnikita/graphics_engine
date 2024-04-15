module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module image_3d.descriptor_set_layout;

namespace ge::image3d
{
    export vk::UniqueDescriptorSetLayout create_descriptor_set_layout(const vk::Device&);
}
