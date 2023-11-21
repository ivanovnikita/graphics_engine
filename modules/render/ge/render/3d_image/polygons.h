#pragma once

#include "ge/geometry/texture_coords.h"
#include "ge/geometry/world_3d_coords.h"

#include "ge/io/model.h"

#include <array>
#include <vector>

namespace ge::image3d
{
    // TODO: use Model instead?
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

    Polygons from_model(const Model&);
}
