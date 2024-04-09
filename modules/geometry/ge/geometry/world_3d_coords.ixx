module;

#include <glm/glm.hpp>

export module world_3d_coords;

namespace ge
{
    export struct World3dCoords final
    {
        glm::vec3 coords;
    };
}
