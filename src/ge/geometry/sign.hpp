#pragma once

namespace ge
{
    template <typename T>
    int sign(T val) noexcept
    {
        return (T(0) < val) - (val < T(0));
    }
}
