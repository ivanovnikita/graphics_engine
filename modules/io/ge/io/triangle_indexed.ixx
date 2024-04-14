module;

#include <array>

#include <cstddef>

export module io.triangle_indexed;

namespace ge
{
    export struct TriangleIndexed final
    {
        std::array<size_t, 3> inds;
    };
}
