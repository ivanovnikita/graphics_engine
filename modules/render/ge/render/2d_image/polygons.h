#pragma once

#include "ge/render/vertex.h"
#include "ge/render/texture_coord.h"

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace ge::image
{
    struct Polygons final
    {
        struct Triangle final
        {
            std::array<size_t, 3> inds;
            TextureCoord tex_coord;
        };

        std::vector<Vertex> points;
        std::vector<Triangle> triangles;
    };
}
