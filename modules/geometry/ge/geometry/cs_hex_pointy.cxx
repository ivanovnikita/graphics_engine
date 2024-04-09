module cs_hex_pointy;

namespace ge
{
    int HexCoordAxialPointy::z() const noexcept
    {
        return -x - y;
    }

    HexCoordOffsetPointy to_hex_offset_pointy(const HexCoordDoubledWidth& in) noexcept
    {
        if ((in.y & 1) == 0)
        {
            return {in.x / 2, in.y};
        }
        else
        {
            return {(in.x - 1) / 2, in.y};
        }
    }

    HexCoordDoubledWidth to_hex_doubled_width(const HexCoordOffsetPointy& in) noexcept
    {
        if ((in.y & 1) == 0)
        {
            return {2 * in.x, in.y};
        }
        else
        {
            return {2 * in.x + 1, in.y};
        }
    }

    HexCoordDoubledWidth to_hex_doubled_width(const HexCoordAxialPointy& in) noexcept
    {
        return {2 * in.x + in.y, in.y};
    }

    HexCoordOffsetPointy to_hex_offset_pointy(const HexCoordAxialPointy& in) noexcept
    {
        return {in.x + ((in.y - (in.y & 1)) / 2), in.y};
    }

    HexCoordAxialPointy to_hex_axial_pointy(const HexCoordDoubledWidth& in) noexcept
    {
        return {(in.x - in.y) / 2, in.y};
    }

    HexCoordAxialPointy to_hex_axial_pointy(const HexCoordOffsetPointy& in) noexcept
    {
        return {in.x - (in.y - (in.y & 1)) / 2, in.y};
    }
}
