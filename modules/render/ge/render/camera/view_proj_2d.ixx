module;

#include <glm/vec2.hpp>

export module view_proj_2d;

export import world_2d_coords;

namespace ge
{
    export struct ViewProj2d final
    {
        World2dCoords camera_pos;
        glm::vec2 ortho_proj; // https://jsantell.com/3d-projection/#orthographic-projection
    };
}
