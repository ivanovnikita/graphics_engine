#include "convert_between_flat_and_pointy.h"

namespace ge
{
    HexCoordAxialFlat to_flat_by_ccw_rotation(const HexCoordAxialPointy& in) noexcept
    {
        return {in.x, in.y};
    }

    HexCoordAxialPointy to_pointy_by_cw_rotation(const HexCoordAxialFlat& in) noexcept
    {
        return {in.x, in.y};
    }
}
