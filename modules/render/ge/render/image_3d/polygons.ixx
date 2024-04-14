module;

#include <array>
#include <vector>

export module image_3d.polygons;

export import texture_coords;
export import world_3d_coords;
export import io.model;

namespace ge::image3d
{
    // TODO: use Model instead?
    export struct Polygons final
    {
        struct TexturedVertex final
        {
            World3dCoords world_coords;
            TextureCoords texture_coords;
        };

        struct Triangle final
        {
            std::array<size_t, 3> inds;
        };

        std::vector<TexturedVertex> vertices;
        std::vector<Triangle> triangles;
    };

    export Polygons from_model(const Model&);
}
