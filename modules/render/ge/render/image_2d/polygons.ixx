module;

#include <glm/glm.hpp>

#include <array>
#include <vector>

export module image_2d.polygons;

export import texture_coords;
export import world_2d_coords;

namespace ge::image2d
{
    export struct Polygons final
    {
        struct TexturedVertex final
        {
            World2dCoords coord;
            TextureCoords tex_coord;
        };

        struct Triangle final
        {
            std::array<size_t, 3> vertices;
        };

        std::vector<TexturedVertex> vertices;
        std::vector<Triangle> triangles;
    };
}
