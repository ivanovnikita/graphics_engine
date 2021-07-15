#pragma once

#include "cs_square_pointy.hpp"
#include "point_localization.hpp"
#include "sign.hpp"

namespace ge
{
    template <typename T>
    CsSquarePointy<T>::CsSquarePointy
    (
        T width,
        T height,
        T y_tilt
    ) noexcept
        : width_{width}
        , height_{height}
        , y_tilt_{y_tilt}
    {
    }

    template <typename T>
    SquareCoordAxialPointy CsSquarePointy<T>::to_axial(const Point2d<T>& in) const noexcept
    {
        const Point2d<T> shifted_origin
        {
            in.x + width_ / 2,
            in.y + ((height_ / 2) + (y_tilt_ / 2))
        };

        const int x = static_cast<int>(std::floor(shifted_origin.x / width_));

        const T y_shifted = shifted_origin.y - (shifted_origin.x / width_) * y_tilt_;
        const int y = static_cast<int>(std::floor(y_shifted / height_));

        return {x, y};
    }

    template <typename T>
    Point2d<T> CsSquarePointy<T>::to_draw_space(const SquareCoordAxialPointy& in) const noexcept
    {
        const T x = static_cast<T>(in.x) * width_;
        const T y = static_cast<T>(in.y) * height_ + in.x * y_tilt_;

        return {x, y};
    }
}
