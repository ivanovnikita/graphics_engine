#pragma once

namespace ge
{
    template <typename T>
    concept Coord2dLike = requires
    (T c)
    {
        c.x;
        c.y;
    };
}
