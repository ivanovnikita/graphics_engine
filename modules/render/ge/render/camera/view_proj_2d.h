#pragma once

#include "ge/render/coords.h"

#include <glm/vec2.hpp>

namespace ge
{
    struct ViewProj2d final
    {
        World2dCoords camera_pos;
        glm::vec2 ortho_proj; // https://jsantell.com/3d-projection/#orthographic-projection
    };
}
