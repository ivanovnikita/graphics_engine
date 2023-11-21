#pragma once

#include <array>

#include <cstddef>

namespace ge
{
    struct TriangleIndexed final
    {
        std::array<size_t, 3> inds;
    };
}
