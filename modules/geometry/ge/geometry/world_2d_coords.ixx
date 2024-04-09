module;

#include <glm/glm.hpp>

export module world_2d_coords;

namespace ge
{
    export struct World2dCoords final
    {
        glm::vec2 coords;
    };
}
