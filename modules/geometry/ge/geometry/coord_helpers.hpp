#pragma once

#include "concepts.hpp"

#include <iostream>

namespace ge
{
    template <typename T>
        requires Coord2dLike<T>
    bool operator==(const T&, const T&) noexcept;

    template <typename T>
        requires Coord2dLike<T>
    bool operator!=(const T&, const T&) noexcept;

    template <typename T>
        requires Coord2dLike<T>
    std::ostream& operator<<(std::ostream&, const T&);
}

#include "coord_helpers_impl.hpp"
