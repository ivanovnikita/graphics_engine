module;

#include <vulkan/vulkan.hpp>

export module graph_2d.descriptor_pool;

namespace ge::graph
{
    export vk::UniqueDescriptorPool create_descriptor_pool
    (
        const vk::Device&,
        size_t size
    );
}
