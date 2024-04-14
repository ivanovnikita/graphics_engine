module;

#include <span>

#include <cstddef>

export module graph_2d.graph_2d;

export import color;
export import world_2d_coords;

namespace ge::graph
{
    export struct Graph final
    {
        struct Vertice final
        {
            size_t index;
            Color color;
        };

        struct Arc final
        {
            size_t index_from;
            size_t index_to;
            Color color;
        };

        std::span<const World2dCoords> points;

        std::span<const Vertice> vertices;
        std::span<const Arc> arcs;
    };
}
