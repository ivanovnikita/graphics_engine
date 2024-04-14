module;

#include <glm/glm.hpp>

export module color;

namespace ge
{
    export struct Color final
    {
        glm::vec3 color;
    };
}
