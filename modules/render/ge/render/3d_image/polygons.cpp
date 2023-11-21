#include "polygons.h"

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
