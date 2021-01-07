#pragma once

#include "ge/geometry/concepts.hpp"
#include "point.hpp"

#include <iostream>

#include <cstdint>

namespace ge
{
    // doubled height coords: https://www.redblobgames.com/grids/hexagons/#coordinates-doubled
    struct HexCoordDoubledHeight final
    {
        int x;
        int y;
    };

    // offset coords in vertical layout: https://www.redblobgames.com/grids/hexagons/#coordinates-offset
    struct HexCoordOffsetFlat final
    {
        int x;
        int y;
    };

    struct HexCoordAxialFlat final
    {
        int x;
        int y;

        int z() const noexcept;
    };

    HexCoordOffsetFlat to_hex_offset_flat(const HexCoordDoubledHeight&) noexcept;
    HexCoordOffsetFlat to_hex_offset_flat(const HexCoordAxialFlat&) noexcept;

    HexCoordDoubledHeight to_hex_doubled_height(const HexCoordOffsetFlat&) noexcept;
    HexCoordDoubledHeight to_hex_doubled_height(const HexCoordAxialFlat&) noexcept;

    HexCoordAxialFlat to_hex_axial_flat(const HexCoordDoubledHeight&) noexcept;
    HexCoordAxialFlat to_hex_axial_flat(const HexCoordOffsetFlat&) noexcept;

    template <typename T>
        requires Coord2dLike<T>
    bool operator==(const T&, const T&) noexcept;

    template <typename T>
        requires Coord2dLike<T>
    bool operator!=(const T&, const T&) noexcept;

    template <typename T>
        requires Coord2dLike<T>
    std::ostream& operator<<(std::ostream&, const T&);

    // Opposite sides of hex are parallel and symmetric
    // Origin of draw space CS is in the center of hex (0; 0)
    template <typename T>
    class CsHexFlat final
    {
    public:
        // x_0 < x_1 < x_2 < x_3
        explicit CsHexFlat
        (
            T width,
            T height,
            T x_1,
            T x_2
        ) noexcept;

        HexCoordDoubledHeight to_hex_doubled_height(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const HexCoordDoubledHeight&) const noexcept;

        HexCoordAxialFlat to_hex_axial_flat(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const HexCoordAxialFlat&) const noexcept;

    private:
        T width_;
        T height_;
        T x_1_;
        T x_2_;

        T width_cycle_;
        T width_half_;

        T height_half_;

        T last_x_;
        T pre_last_x_;
    };
}

#include "cs_hex_flat_impl.hpp"
