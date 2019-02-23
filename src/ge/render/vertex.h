#pragma once

#include "ge/render/utils/span.hpp"

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace ge
{
    struct Vertex final
    {
        glm::vec2 pos;
        glm::vec3 color;

        static vk::VertexInputBindingDescription binding_description();
        static Span<const vk::VertexInputAttributeDescription> attribute_descriptions();
    };
}
