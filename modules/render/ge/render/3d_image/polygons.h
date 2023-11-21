#pragma once

#include "ge/geometry/texture_coords.h"
#include "ge/geometry/world_3d_coords.h"

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace ge::image3d
{
    struct Polygons final
    {
        struct TexturedVertex final
        {
            World3dCoords world_coords;
            TextureCoords texture_coords;
        };

        struct Triangle final
        {
            std::array<size_t, 3> inds;
        };

        std::vector<TexturedVertex> vertices;
        std::vector<Triangle> triangles;
    };
}
