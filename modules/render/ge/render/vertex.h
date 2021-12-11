#pragma once

#include <glm/glm.hpp>

#include <variant>
#include <span>
#include <vector>

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

    struct Polygons final
    {
        struct Triangle final
        {
            std::array<size_t, 3> inds;
            Color color;
        };

        struct Line final
        {
            std::array<size_t, 2> inds;
            Color color;
        };

        std::vector<Vertex> points;
        std::vector<Triangle> triangles;
        std::vector<Line> lines;
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
