#pragma once

#include "point.hpp"

#include <cstdint>

namespace ge
{
    // doubled coords: https://www.redblobgames.com/grids/hexagons/#coordinates-doubled
    struct HexCoordDoubled final
    {
        int x;
        int y;
    };

    bool operator==(const HexCoordDoubled&, const HexCoordDoubled&) noexcept;
    bool operator!=(const HexCoordDoubled&, const HexCoordDoubled&) noexcept;

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
        ) noexcept;

        HexCoordDoubled convert(const Point2d<T>&) const noexcept;

        Point2d<T> convert(const HexCoordDoubled&) const noexcept;

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
