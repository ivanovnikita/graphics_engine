#pragma once

#include <glm/mat4x4.hpp>

namespace ge
{
    struct Camera2D final
    {
        glm::vec2 pos;
        float scale;
    };
}
