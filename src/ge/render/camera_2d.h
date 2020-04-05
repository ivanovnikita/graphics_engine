#pragma once

#include <glm/mat4x4.hpp>

namespace ge
{
    struct Camera2D final
    {
        glm::vec2 pos;
        glm::vec2 ortho_proj; // https://jsantell.com/3d-projection/#orthographic-projection
        float scale;
    };
}
