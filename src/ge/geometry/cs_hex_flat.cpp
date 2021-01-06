#include "cs_hex_flat.hpp"

namespace ge
{
    int HexCoordAxialFlat::z() const noexcept
    {
        return -x - y;
    }

    HexCoordOffsetFlat to_hex_offset_flat(const HexCoordDoubledHeight& in) noexcept
    {
        if ((in.x & 1) == 0)
        {
            return {in.x, in.y / 2};
        }
        else
        {
            return {in.x, (in.y - 1) / 2};
        }
    }

    HexCoordDoubledHeight to_hex_doubled_height(const HexCoordOffsetFlat& in) noexcept
    {
        if ((in.x & 1) == 0)
        {
            return {in.x, 2 * in.y};
        }
        else
        {
            return {in.x, 2 * in.y + 1};
        }
    }

    HexCoordDoubledHeight to_hex_doubled_height(const HexCoordAxialFlat& in) noexcept
    {
        return {in.x, 2 * in.y + in.x};
    }

    HexCoordOffsetFlat to_hex_offset_flat(const HexCoordAxialFlat& in) noexcept
    {
        return {in.x, in.y + ((in.x - (in.x & 1)) / 2)};
    }

    HexCoordAxialFlat to_hex_axial_flat(const HexCoordDoubledHeight& in) noexcept
    {
        return {in.x, (in.y - in.x) / 2};
    }

    HexCoordAxialFlat to_hex_axial_flat(const HexCoordOffsetFlat& in) noexcept
    {
        return {in.x, in.y - (in.x - (in.x & 1)) / 2};
    }
}
