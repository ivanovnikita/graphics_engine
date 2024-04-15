module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

#include <array>

export module tiles_2d.vertex_description;

export import logger;

namespace ge::tiles
{
    constexpr size_t VERTEX_DESCRIPTION_COUNT = 2;

    // Vertex + Color
    export std::array
    <
        vk::VertexInputBindingDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_binding_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );

    export std::array
    <
        vk::VertexInputAttributeDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_attribute_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );
}
