#pragma once

#include "hex_coord_rotate.hpp"

namespace ge
{
    // https://www.redblobgames.com/grids/hexagons/#rotation
    template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_ccw(const T& in) noexcept
    {
        return {-in.z(), -in.x};
    }

    template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_cw(const T& in) noexcept
    {
        return {-in.y, -in.z()};
    }
}
