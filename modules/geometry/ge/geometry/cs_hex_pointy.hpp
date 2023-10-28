#pragma once

#include "concepts.hpp"
#include "coord_helpers.hpp"
#include "point.hpp"

#include <iostream>

#include <cstdint>

namespace ge
{
    // doubled width coords: https://www.redblobgames.com/grids/hexagons/#coordinates-doubled
    struct HexCoordDoubledWidth final
    {
        int x;
        int y;
    };

    // offset odd-r coords in horizontal layout: https://www.redblobgames.com/grids/hexagons/#coordinates-offset
    struct HexCoordOffsetPointy final
    {
        int x;
        int y;
    };

    struct HexCoordAxialPointy final
    {
        int x;
        int y;

        int z() const noexcept;
    };

    HexCoordOffsetPointy to_hex_offset_pointy(const HexCoordDoubledWidth&) noexcept;
    HexCoordOffsetPointy to_hex_offset_pointy(const HexCoordAxialPointy&) noexcept;

    HexCoordDoubledWidth to_hex_doubled_width(const HexCoordOffsetPointy&) noexcept;
    HexCoordDoubledWidth to_hex_doubled_width(const HexCoordAxialPointy&) noexcept;

    HexCoordAxialPointy to_hex_axial_pointy(const HexCoordDoubledWidth&) noexcept;
    HexCoordAxialPointy to_hex_axial_pointy(const HexCoordOffsetPointy&) noexcept;

    // Opposite sides of hex are parallel and symmetric
    // Origin of draw space CS is in the center of hex (0; 0)
    template <typename T>
    class CsHexPointy final
    {
    public:
        // y_0 < y_1 < y_2 < y_3
        explicit CsHexPointy
        (
            T width,
            T height,
            T y_1,
            T y_2
        ) noexcept;

        HexCoordDoubledWidth to_hex_doubled_width(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const HexCoordDoubledWidth&) const noexcept;

        HexCoordAxialPointy to_hex_axial_pointy(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const HexCoordAxialPointy&) const noexcept;

    private:
        T width_;
        T height_;
        T y_1_;
        T y_2_;

        T height_cycle_;
        T height_half_;

        T width_half_;

        T last_y_;
        T pre_last_y_;
    };

    template <typename T>
    CsHexPointy(T, T, T, T) -> CsHexPointy<T>;
}

#include "cs_hex_pointy_impl.hpp"
