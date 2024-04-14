module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.descriptor_pool;

namespace ge::tiles
{
    export vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}
