module;

#include <glm/glm.hpp>

export module surface_coords;

namespace ge
{
    export struct SurfaceCoords final
    {
        glm::vec2 coords;
    };
}
