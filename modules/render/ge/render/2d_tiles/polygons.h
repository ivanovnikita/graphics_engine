#pragma once

#include "ge/render/vertex.h"

#include <vector>

namespace ge::tiles
{
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
}
