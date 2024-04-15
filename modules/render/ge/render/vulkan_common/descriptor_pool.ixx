module;

#include "vulkan_fwds.h"

#include <span>

export module vulkan_common.descriptor_pool;

namespace ge
{
    export vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size,
        std::span<const vk::DescriptorPoolSize>
    );
}
