#pragma once

#include "ge/render/vertex.h"

#include <span>

#include <cstddef>

namespace ge//::graph
{
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
}
