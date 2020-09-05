#include "vertex_description.h"

#include <array>

namespace ge
{
    std::span<const vk::VertexInputBindingDescription> vertex_binding_description()
    {
        static const std::array descriptions
        {
            vk::VertexInputBindingDescription{}
                .setBinding(0)
                .setStride(sizeof(Vertex))
                .setInputRate(vk::VertexInputRate::eVertex)
            , vk::VertexInputBindingDescription{}
                .setBinding(1)
                .setStride(sizeof(Color))
                .setInputRate(vk::VertexInputRate::eVertex)
        };

        return descriptions;
    }

    std::span<const vk::VertexInputAttributeDescription> vertex_attribute_descriptions()
    {
        // https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap39.html#_identification_of_formats
        static const std::array descriptions
        {
            vk::VertexInputAttributeDescription{}
                .setBinding(0)
                .setLocation(0)
                .setFormat(vk::Format::eR32G32Sfloat)
                .setOffset(offsetof(Vertex, pos))
            , vk::VertexInputAttributeDescription{}
                .setBinding(1)
                .setLocation(1)
                .setFormat(vk::Format::eR32G32B32Sfloat)
                .setOffset(offsetof(Color, color))
        };

        return descriptions;
    }
}
