#pragma once

#include "ge/render/vertex.h"
#include "ge/render/utils/span.hpp"

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::VertexInputBindingDescription vertex_binding_description();
    Span<const vk::VertexInputAttributeDescription> vertex_attribute_descriptions();
}

