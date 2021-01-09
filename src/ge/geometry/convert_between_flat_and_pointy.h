#pragma once

#include "cs_hex_flat.hpp"
#include "cs_hex_pointy.hpp"

namespace ge
{
    HexCoordAxialFlat to_flat_by_ccw_rotation(const HexCoordAxialPointy&) noexcept;

    HexCoordAxialPointy to_pointy_by_cw_rotation(const HexCoordAxialFlat&) noexcept;
}
