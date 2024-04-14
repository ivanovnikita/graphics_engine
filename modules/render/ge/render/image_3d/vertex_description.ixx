module;

#include <vulkan/vulkan.hpp>

export module image_3d.vertex_description;

export import logger;

namespace ge::image3d
{
    // Vertex + Color
    export std::array
    <
        vk::VertexInputBindingDescription,
        1
    > get_vertex_binding_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );

    export std::array
    <
        vk::VertexInputAttributeDescription,
        2
    > get_vertex_attribute_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );
}
