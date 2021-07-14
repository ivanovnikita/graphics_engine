#pragma once

#include "coord_helpers.hpp"
#include "point.hpp"

namespace ge
{
    struct SquareCoordAxialFlat final
    {
        int x;
        int y;
    };

    template <typename T>
    class CsSquareFlat final
    {
    public:
        explicit CsSquareFlat
        (
            T width,
            T height,
            T x_1
        ) noexcept;

        SquareCoordAxialFlat to_axial(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const SquareCoordAxialFlat&) const noexcept; // square center

    private:
        T width_;
        T height_;
        T x_1_;
    };
}

#include "cs_square_flat_impl.hpp"
