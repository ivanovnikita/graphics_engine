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
            World2dCoords coord;
            TextureCoord tex_coord;
        };

        struct Triangle final
        {
            std::array<size_t, 3> vertices;
        };

        std::vector<TexturedVertex> vertices;
        std::vector<Triangle> triangles;
    };
}
