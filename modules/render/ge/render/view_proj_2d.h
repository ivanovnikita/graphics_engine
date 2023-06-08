#pragma once

#include <glm/vec2.hpp>

namespace ge
{
    struct ViewProj2d final
    {
        glm::vec2 pos;
        glm::vec2 ortho_proj; // https://jsantell.com/3d-projection/#orthographic-projection
    };
}
