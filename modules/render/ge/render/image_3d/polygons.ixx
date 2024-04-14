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

module : private;

namespace ge::image3d
{
    Polygons from_model(const Model& model)
    {

        std::vector<Polygons::TexturedVertex> vertices;
        vertices.reserve(model.vertices().size());

        std::vector<Polygons::Triangle> triangles;
        triangles.reserve(model.triangles().size());

        for (const Vertex& vertex : model.vertices())
        {
            vertices.emplace_back
            (
                Polygons::TexturedVertex
                {
                    .world_coords = vertex.world_coords,
                    .texture_coords = vertex.texture_coords
                }
            );
        }

        for (const TriangleIndexed& triangle : model.triangles())
        {
            triangles.emplace_back(Polygons::Triangle{triangle.inds});
        }

        return Polygons
        {
            .vertices = std::move(vertices),
            .triangles = std::move(triangles)
        };
    }
}
