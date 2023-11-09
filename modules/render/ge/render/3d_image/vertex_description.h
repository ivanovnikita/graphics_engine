#pragma once

#include "ge/common/logger.hpp"

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    // Vertex + Color
    std::array
    <
        vk::VertexInputBindingDescription,
        1
    > get_vertex_binding_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );

    std::array
    <
        vk::VertexInputAttributeDescription,
        2
    > get_vertex_attribute_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );
}
