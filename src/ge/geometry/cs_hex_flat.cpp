#include "cs_hex_flat.hpp"

namespace ge
{
    bool operator==(const HexCoordDoubledHeight& a, const HexCoordDoubledHeight& b) noexcept
    {
        return a.x == b.x and a.y == b.y;
    }

    bool operator!=(const HexCoordDoubledHeight& a, const HexCoordDoubledHeight& b) noexcept
    {
        return not (a == b);
    }
}
