#pragma once

#include "ge/geometry/world_3d_coords.h"
#include "ge/geometry/texture_coords.h"

namespace ge
{
    struct Vertex final
    {
        World3dCoords world_coords;
        TextureCoords texture_coords;
    };
}
