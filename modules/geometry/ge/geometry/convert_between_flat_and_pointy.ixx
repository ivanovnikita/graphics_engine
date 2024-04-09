export module convert_between_flat_and_pointy;

import cs_hex_flat;
import cs_hex_pointy;

namespace ge
{
    export HexCoordAxialFlat to_flat_by_ccw_rotation(const HexCoordAxialPointy&) noexcept;

    export HexCoordAxialPointy to_pointy_by_cw_rotation(const HexCoordAxialFlat&) noexcept;
}
