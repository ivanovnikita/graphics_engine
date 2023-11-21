#pragma once

#include <glm/glm.hpp>

namespace ge
{
    // Normalized in interval [-1; 1] for each axis
    struct SurfaceNormalizedCoords final
    {
        glm::vec2 coords;
    };
}
