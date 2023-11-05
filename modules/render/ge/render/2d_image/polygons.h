#pragma once

#include "ge/render/coords.h"
#include "ge/render/texture_coord.h"

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace ge::image2d
{
    struct Polygons final
    {
        struct TexturedVertex final
        {
            size_t index;
            TextureCoord tex_coord;
        };

        struct Triangle final
        {
            std::array<TexturedVertex, 3> vertices;
        };

        std::vector<World2dCoords> points;
        std::vector<Triangle> triangles;
    };
}
