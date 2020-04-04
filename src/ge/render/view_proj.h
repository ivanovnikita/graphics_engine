#pragma once

#include <glm/mat4x4.hpp>

namespace ge
{
    struct ViewProj final
    {
        glm::mat4 view;
        glm::mat4 proj;
    };
}
