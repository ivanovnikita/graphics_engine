#include "vertex.h"

#include <array>

namespace ge
{
    vk::VertexInputBindingDescription Vertex::binding_description()
    {
        return vk::VertexInputBindingDescription{}
            .setBinding(0)
            .setStride(sizeof(Vertex))
            .setInputRate(vk::VertexInputRate::eVertex);
    }

    Span<const vk::VertexInputAttributeDescription> Vertex::attribute_descriptions()
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
