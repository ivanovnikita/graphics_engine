#pragma once

#include <glm/glm.hpp>

#include <variant>

namespace ge
{
    struct Vertex final
    {
        glm::vec2 pos;
    };

    struct Color final
    {
        glm::vec3 color;
    };

    struct VerticesArePolygons final
    {
    };

    template <typename T>
    struct Required final
    {
        T value;
    };

    template <typename T>
    struct Optional final
    {
        T value;
    };

    template <typename T>
    using Param = std::variant<Required<T>, Optional<T>>;

    struct VerticesAreLines final
    {
        Param<uint8_t> line_width;
    };

    using VerticesInterpretation = std::variant
    <
        VerticesArePolygons
        , VerticesAreLines
    >;
}
