module;

#include <array>
#include <vector>

export module tiles_2d.polygons;

export import color;
export import world_2d_coords;

namespace ge::tiles
{
    export struct Polygons final
    {
        struct Triangle final
        {
            std::array<size_t, 3> inds;
            Color color;
        };

        struct Line final
        {
            std::array<size_t, 2> inds;
            Color color;
        };

        std::vector<World2dCoords> points;
        std::vector<Triangle> triangles;
        std::vector<Line> lines;
    };
}
