#pragma once

#include "coord_helpers.hpp"
#include "point.hpp"

namespace ge
{
    struct SquareRightCoordAxial final
    {
        int x;
        int y;
    };

    template <typename T>
    class CsSquareRightFlat final
    {
    public:
        explicit CsSquareRightFlat
        (
            T width,
            T height,
            T x_1
        ) noexcept;

        SquareRightCoordAxial to_axial(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const SquareRightCoordAxial&) const noexcept; // square center

    private:
        T width_;
        T height_;
        T x_1_;
    };
}

#include "cs_square_right_flat_impl.hpp"
