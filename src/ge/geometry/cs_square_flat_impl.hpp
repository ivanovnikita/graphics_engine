#pragma once

#include "cs_square_flat.hpp"
#include "point_localization.hpp"
#include "sign.hpp"

namespace ge
{
    template <typename T>
    CsSquareFlat<T>::CsSquareFlat
    (
        T width,
        T height,
        T x_1
    ) noexcept
        : width_{width}
        , height_{height}
        , x_1_{x_1}
    {
    }

    template <typename T>
    SquareCoordAxialFlat CsSquareFlat<T>::to_axial(const Point2d<T>& in) const noexcept
    {
        const Point2d<T> shifted_origin
        {
            in.x + ((width_ / 2) + (x_1_ / 2)),
            in.y + height_ / 2
        };

        const T x_left_shifted = shifted_origin.x - (shifted_origin.y / height_) * x_1_;
        const int x = static_cast<int>(std::floor(x_left_shifted / width_));

        const int y = static_cast<int>(std::floor(shifted_origin.y / height_));

        return {x, y};
    }

    template <typename T>
    Point2d<T> CsSquareFlat<T>::to_draw_space(const SquareCoordAxialFlat& in) const noexcept
    {
        const T y = static_cast<T>(in.y) * height_;
        const T x = static_cast<T>(in.x) * width_ + (y / height_) * x_1_;

        return {x, y};
    }
}
