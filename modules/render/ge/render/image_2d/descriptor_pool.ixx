module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

#include <cstddef>

export module image_2d.descriptor_pool;

namespace ge::image2d
{
    export vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}
