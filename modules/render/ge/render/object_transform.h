#pragma once

#include "ge/geometry/world_3d_coords.h"

namespace ge
{
    struct ObjectTransform final
    {
        World3dCoords pos;
        glm::vec3 rotate; // x - rotate around axis X, in degress
        glm::vec3 scale;
    };
}
