#pragma once

#include "ge/geometry/texture_coords.h"
#include "ge/geometry/world_2d_coords.h"

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace ge::image2d
{
    struct Polygons final
    {
        struct TexturedVertex final
        {
            World2dCoords coord;
            TextureCoords tex_coord;
        };

        struct Triangle final
        {
            std::array<size_t, 3> vertices;
        };

        std::vector<TexturedVertex> vertices;
        std::vector<Triangle> triangles;
    };
}
