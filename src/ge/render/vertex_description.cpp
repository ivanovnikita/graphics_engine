#include "vertex_description.h"

#include <array>

namespace ge
{
    vk::VertexInputBindingDescription vertex_binding_description()
    {
        return vk::VertexInputBindingDescription{}
            .setBinding(0) // TODO: split into several bindings: points, line colors, points colors
            .setStride(sizeof(Vertex))
            .setInputRate(vk::VertexInputRate::eVertex);
    }

    std::span<const vk::VertexInputAttributeDescription> vertex_attribute_descriptions()
    {
        static const std::array descriptions
        {
            vk::VertexInputAttributeDescription{}
                .setBinding(0)
                .setLocation(0)
                .setFormat(vk::Format::eR32G32Sfloat)
                .setOffset(offsetof(Vertex, pos))
            , vk::VertexInputAttributeDescription{}
                .setBinding(0)
                .setLocation(1)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(offsetof(Vertex, color))
        };

        return descriptions;
    }
}
