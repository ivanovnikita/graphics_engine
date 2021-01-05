#include "cs_hex.hpp"

namespace ge
{
    bool operator==(const CoordHex& a, const CoordHex& b) noexcept
    {
        return a.x == b.x and a.y == b.y;
    }

    bool operator!=(const CoordHex& a, const CoordHex& b) noexcept
    {
        return not (a == b);
    }
}
