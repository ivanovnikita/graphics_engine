module;

#include <glm/glm.hpp>

export module surface_normalized_coords;

namespace ge
{
    // Normalized in interval [-1; 1] for each axis
    export struct SurfaceNormalizedCoords final
    {
        glm::vec2 coords;
    };
}
