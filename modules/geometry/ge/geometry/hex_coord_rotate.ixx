module;

#include <type_traits>

export module hex_coord_rotate;

import cs_hex_flat;
import cs_hex_pointy;

namespace ge
{
    // 60° angle rotation around CS origin

    export template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_ccw(const T&) noexcept;

    export template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_cw(const T&) noexcept;
}

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

