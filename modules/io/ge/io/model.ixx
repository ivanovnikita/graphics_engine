module;

#include <span>
#include <vector>

export module io.model;

export import io.vertex;
export import io.triangle_indexed;

namespace ge
{
    export class Model final
    {
    public:
        explicit Model
        (
            std::vector<Vertex>,
            std::vector<TriangleIndexed>
        );

        std::span<const Vertex> vertices() const;
        std::span<const TriangleIndexed> triangles() const;

    private:
        std::vector<Vertex> vertices_;
        std::vector<TriangleIndexed> triangles_;
    };
}
