#pragma once

#include "cs_hex_flat.hpp"
#include "cs_hex_pointy.hpp"

#include "type_traits"

namespace ge
{
    // 60° angle rotation around CS origin

    template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_ccw(const T&) noexcept;

    template <typename T>
        requires std::is_same_v<T, HexCoordAxialFlat> or std::is_same_v<T, HexCoordAxialPointy>
    T rotate_cw(const T&) noexcept;
}

#include "hex_coord_rotate_impl.hpp"
