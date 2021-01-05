#include "cs_hex.hpp"

namespace ge
{
    bool operator==(const HexCoordDoubled& a, const HexCoordDoubled& b) noexcept
    {
        return a.x == b.x and a.y == b.y;
    }

    bool operator!=(const HexCoordDoubled& a, const HexCoordDoubled& b) noexcept
    {
        return not (a == b);
    }
}
