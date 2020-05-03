#pragma once

#include <glm/glm.hpp>

#include <variant>
#include <span>

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

    struct Graph final
    {
        struct Vertice final
        {
            size_t index;
            Color color;
        };

        struct Arc final
        {
            size_t index_from;
            size_t index_to;
            Color color;
        };

        std::span<const Vertex> points;

        std::span<const Vertice> vertices;
        std::span<const Arc> arcs;
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
}
