module;

#include <glm/glm.hpp>

export module texture_coords;

namespace ge
{
    export struct TextureCoords final
    {
        glm::vec2 coords;
    };
}
