#pragma once

#include "coord_helpers.hpp"

namespace ge
{
    template <typename T>
        requires Coord2dLike<T>
    bool operator==(const T& a, const T& b) noexcept
    {
        return a.x == b.x and a.y == b.y;
    }

    template <typename T>
        requires Coord2dLike<T>
    bool operator!=(const T& a, const T& b) noexcept
    {
        return not (a == b);
    }

    template <typename T>
        requires Coord2dLike<T>
    std::ostream& operator<<(std::ostream& out, const T& v)
    {
        out << "{" << v.x << ", " << v.y << "}";
        return out;
    }
}
