#pragma once

#include <glm/glm.hpp>

namespace ge
{
    struct World2dCoords final
    {
        glm::vec2 coords;
    };

    // Normalzied in interval [-1; 1] for each axis
    struct SurfaceNormalizedCoords final
    {
        glm::vec2 coords;
    };
}
