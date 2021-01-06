#include "cs_hex_flat.hpp"

namespace ge
{
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
}
