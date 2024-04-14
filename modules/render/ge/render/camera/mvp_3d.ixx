module;

#include <glm/mat4x4.hpp>

export module mvp_3d;

namespace ge
{
    export struct Mvp3d final
    {
        glm::mat4 mvp;
    };
}
