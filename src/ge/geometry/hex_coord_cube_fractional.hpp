#pragma once

namespace ge
{
    template <typename T>
    struct HexCoordCubeFractional final
    {
        T x;
        T y;
        T z;
    };

    template <typename R, typename T>
    R hex_round(const HexCoordCubeFractional<T>&) noexcept;
}

#include "hex_coord_cube_fractional_impl.hpp"
