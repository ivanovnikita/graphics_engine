#pragma once

#include "ge/render/vertex.h"

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::VertexInputBindingDescription vertex_binding_description();
    std::span<const vk::VertexInputAttributeDescription> vertex_attribute_descriptions();
}

