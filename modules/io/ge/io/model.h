#pragma once

#include "vertex.h"
#include "triangle_indexed.h"

#include <span>
#include <vector>

namespace ge
{
    class Model final
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
