#pragma once

#include "coord_helpers.hpp"
#include "point.hpp"

#include <optional>

namespace ge
{
    struct SquareCoordAxialPointy final
    {
        int x;
        int y;
    };

    template <typename T>
    class CsSquarePointy final
    {
    public:
        explicit CsSquarePointy
        (
            T width,
            T height,
            T y_tilt
        ) noexcept;

        SquareCoordAxialPointy to_axial(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const SquareCoordAxialPointy&) const noexcept; // square center

    private:
        T width_;
        T height_;
        T y_tilt_;
    };
}

#include "cs_square_pointy_impl.hpp"
