#pragma once

#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    // Vertex + Color
    std::span<const vk::VertexInputBindingDescription> vertex_binding_description();

    std::span<const vk::VertexInputAttributeDescription> vertex_attribute_descriptions();
}

