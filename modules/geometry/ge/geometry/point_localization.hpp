#pragma once

#include "point.hpp"
#include "line_ref.hpp"

namespace ge
{
    enum class LineSide
    {
        Left,
        Right,
        Collinear
    };

    template <typename T>
    LineSide line_side(const LineCRef2d<T>&, const Point2d<T>&);

    template <typename T>
    bool is_left_side(const LineCRef2d<T>&, const Point2d<T>&);
}

#include "point_localization_impl.hpp"
