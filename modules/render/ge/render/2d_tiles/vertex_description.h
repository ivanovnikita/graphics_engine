#pragma once

#include "ge/common/logger.hpp"

#include <vulkan/vulkan.hpp>

namespace ge::tiles
{
    constexpr size_t VERTEX_DESCRIPTION_COUNT = 2;

    // Vertex + Color
    std::array
    <
        vk::VertexInputBindingDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_binding_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );

    std::array
    <
        vk::VertexInputAttributeDescription,
        VERTEX_DESCRIPTION_COUNT
    > get_vertex_attribute_descriptions
    (
        const vk::PhysicalDeviceLimits&,
        const Logger&
    );
}
