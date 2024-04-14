module;

#include <vector>
#include <span>

module io.model;

namespace ge
{
    Model::Model
    (
        std::vector<Vertex> v,
        std::vector<TriangleIndexed> t
    )
        : vertices_{std::move(v)}
        , triangles_{std::move(t)}
    {
    }

    std::span<const Vertex> Model::vertices() const
    {
        return vertices_;
    }

    std::span<const TriangleIndexed> Model::triangles() const
    {
        return triangles_;
    }
}
