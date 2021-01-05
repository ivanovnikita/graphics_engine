#pragma once

#include "point.hpp"

#include <cstdint>

namespace ge
{
    struct CoordHex final
    {
        int x;
        int y;
    };

    bool operator==(const CoordHex&, const CoordHex&) noexcept;
    bool operator!=(const CoordHex&, const CoordHex&) noexcept;

    // Opposite sides of hex are parallel and symmetric
    // Origin of floating point CS is in the center of hex (0; 0)
    template <typename T>
    class CsHex final
    {
    public:
        // x_0 < x_1 < x_2 < x_3
        explicit CsHex
        (
            T width,
            T height,
            T x_1,
            T x_2
        );

        CoordHex convert(const Point2d<T>&) const;

        Point2d<T> convert(const CoordHex&) const;

    private:
        T width_;
        T height_;
        T x_1_;
        T x_2_;

        T width_cycle_;
        T width_half_;
        T height_half_;

        T pre_last_x_;
    };
}

#include "cs_hex_impl.hpp"
