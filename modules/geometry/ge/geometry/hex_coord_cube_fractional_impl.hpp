#pragma once

#include "hex_coord_cube_fractional.hpp"

#include <cmath>

namespace ge
{
    // https://www.redblobgames.com/grids/hexagons/implementation.html#rounding
    template <typename R, typename T>
    R hex_round(const HexCoordCubeFractional<T>& in) noexcept
    {
        int x = static_cast<int>(std::round(in.x));
        int y = static_cast<int>(std::round(in.y));
        int z = static_cast<int>(std::round(in.z));
        const double x_diff = std::abs(static_cast<T>(x) - in.x);
        const double y_diff = std::abs(static_cast<T>(y) - in.y);
        const double z_diff = std::abs(static_cast<T>(z) - in.z);
        if (x_diff > y_diff and x_diff > z_diff)
        {
            x = -y - z;
        }
        else if (y_diff > z_diff)
        {
            y = -x - z;
        }
        return {x, y};
    }
}
