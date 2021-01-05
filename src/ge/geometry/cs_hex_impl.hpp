#pragma once

#include "cs_hex.hpp"
#include "point_localization.hpp"

#include <cmath>
#include <cstdlib>

namespace ge
{
    template <typename T>
    int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    CsHex<T>::CsHex
    (
        T width,
        T height,
        T x_1,
        T x_2
    ) noexcept
        : width_{width}
        , height_{height}
        , x_1_{x_1}
        , x_2_{x_2}
        , width_cycle_{width_ + (x_2_ - x_1_)}
        , width_half_{width_ / 2}
        , height_half_{height_ / 2}
        , pre_last_x_{width_half_ + (x_2_ - x_1_)}
    {
    }

    template <typename T>
    HexCoordDoubled CsHex<T>::convert(const Point2d<T>& in) const noexcept
    {
        const int x_quot = static_cast<int>(in.x / width_cycle_);
        const T x_rem = std::fabs(std::fmod(in.x, static_cast<T>(width_cycle_)));

        const int y_quot = static_cast<int>(in.y / height_);
        const T y_rem = std::fabs(std::fmod(in.y, static_cast<T>(height_)));

        int x = 2 * x_quot;
        int y = 2 * y_quot;

        if (x_rem < x_2_)
        {
            if (y_rem >= height_half_)
            {
                y += 2 * sign(in.y);
            }
        }
        else if (x_rem >= x_2_ + width_)
        {
            x += 2 * sign(in.x);

            if (y_rem >= height_half_)
            {
                y += 2 * sign(in.y);
            }
        }
        else if (x_rem >= width_half_)
        {
            if (x_rem < pre_last_x_)
            {
                x += 1 * sign(in.x);
                y += 1 * sign(in.y);
            }
            else
            {
                if (y_rem < height_half_)
                {
                    const Point2d<T> a{pre_last_x_, 0};
                    const Point2d<T> b{width_cycle_, height_half_};
                    if (is_left_side(LineCRef2dF{a, b}, {x_rem, y_rem}))
                    {
                        x += 1 * sign(in.x);
                        y += 1 * sign(in.y);
                    }
                    else
                    {
                        x += 2 * sign(in.x);
                    }
                }
                else
                {
                    const Point2d<T> a{width_cycle_, height_half_};
                    const Point2d<T> b{pre_last_x_, height_};
                    if (is_left_side(LineCRef2dF{a, b}, {x_rem, y_rem}))
                    {
                        x += 1 * sign(in.x);
                        y += 1 * sign(in.y);
                    }
                    else
                    {
                        x += 2 * sign(in.x);
                        y += 2 * sign(in.y);
                    }
                }
            }
        }
        else
        {
            if (y_rem < height_half_)
            {
                const Point2d<T> a{width_half_, 0};
                const Point2d<T> b{x_2_, height_half_};
                if (not is_left_side(LineCRef2dF{a, b}, {x_rem, y_rem}))
                {
                    x += 1 * sign(in.x);
                    y += 1 * sign(in.y);
                }
            }
            else
            {
                const Point2d<T> a{x_2_, height_half_};
                const Point2d<T> b{width_half_, height_};
                if (is_left_side(LineCRef2dF{a, b}, {x_rem, y_rem}))
                {
                    y += 2 * sign(in.y);
                }
                else
                {
                    x += 1 * sign(in.x);
                    y += 1 * sign(in.y);
                }
            }
        }

        return {x, y};
    }

    template <typename T>
    Point2d<T> CsHex<T>::convert(const HexCoordDoubled& in) const noexcept
    {
        T x = sign(in.x) * (std::abs(in.x) / 2) * width_cycle_;
        T y = sign(in.y) * (std::abs(in.y) / 2) * height_;

        if (in.x % 2 != 0)
        {
            x += sign(in.x) * (0.5 * width_ + x_2_);
            y += sign(in.y) * 0.5 * height_;
        }

        return {x, y};
    }
}
