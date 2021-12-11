#pragma once

#include "cs_hex_pointy.hpp"
#include "hex_coord_cube_fractional.hpp"
#include "point_localization.hpp"
#include "sign.hpp"

#include <cmath>
#include <cstdlib>

namespace ge
{
    template <typename T>
    CsHexPointy<T>::CsHexPointy
    (
        T width,
        T height,
        T y_1,
        T y_2
    ) noexcept
        : width_{width}
        , height_{height}
        , y_1_{y_1}
        , y_2_{y_2}
        , height_cycle_{height_ + (y_2_ - y_1_)}
        , height_half_{height_ / 2}
        , width_half_{width_ / 2}
        , last_y_{height_ + y_2_}
        , pre_last_y_{height_half_ + (y_2_ - y_1_)}
    {
    }

    template <typename T>
    HexCoordDoubledWidth CsHexPointy<T>::to_hex_doubled_width(const Point2d<T>& in) const noexcept
    {
        const int x_quot = static_cast<int>(in.x / width_);
        const T x_rem = std::fabs(std::fmod(in.x, width_));

        const int y_quot = static_cast<int>(in.y / height_cycle_);
        const T y_rem = std::fabs(std::fmod(in.y, height_cycle_));

        int x = 2 * x_quot;
        int y = 2 * y_quot;

        if (y_rem < y_2_)
        {
            if (x_rem >= width_half_)
            {
                x += 2 * sign(in.x);
            }
        }
        else if (y_rem >= y_2_ + height_)
        {
            y += 2 * sign(in.y);

            if (x_rem >= width_half_)
            {
                x += 2 * sign(in.x);
            }
        }
        else if (y_rem >= height_half_)
        {
            if (y_rem < pre_last_y_)
            {
                x += 1 * sign(in.x);
                y += 1 * sign(in.y);
            }
            else
            {
                if (x_rem < width_half_)
                {
                    const Point2d<T> a{0, pre_last_y_};
                    const Point2d<T> b{width_half_, last_y_};
                    if (is_left_side(LineCRef2d<T>{a, b}, {x_rem, y_rem}))
                    {
                        y += 2 * sign(in.y);
                    }
                    else
                    {
                        x += 1 * sign(in.x);
                        y += 1 * sign(in.y);
                    }
                }
                else
                {
                    const Point2d<T> a{width_, pre_last_y_};
                    const Point2d<T> b{width_half_, last_y_};
                    if (is_left_side(LineCRef2d<T>{a, b}, {x_rem, y_rem}))
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
            if (x_rem < width_half_)
            {
                const Point2d<T> a{0, height_half_};
                const Point2d<T> b{width_half_, y_2_};
                if (is_left_side(LineCRef2d<T>{a, b}, {x_rem, y_rem}))
                {
                    x += 1 * sign(in.x);
                    y += 1 * sign(in.y);
                }
            }
            else
            {
                const Point2d<T> a{width_half_, y_2_};
                const Point2d<T> b{width_, height_half_};
                if (is_left_side(LineCRef2d<T>{a, b}, {x_rem, y_rem}))
                {
                    x += 1 * sign(in.x);
                    y += 1 * sign(in.y);
                }
                else
                {
                    x += 2 * sign(in.x);
                }
            }
        }

        return {x, y};
    }

    template <typename T>
    Point2d<T> CsHexPointy<T>::to_draw_space(const HexCoordDoubledWidth& in) const noexcept
    {
        T x = (static_cast<T>(in.x) / 2) * width_;
        T y = (static_cast<T>(in.y) / 2) * height_cycle_;

        return {x, y};
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#pixel-to-hex
    template <typename T>
    HexCoordAxialPointy CsHexPointy<T>::to_hex_axial_pointy(const Point2d<T>& in) const noexcept
    {
        const T sqrt_3 = std::sqrt(static_cast<T>(3));
        const Point2d<T> normalized(sqrt_3 * (in.x / width_), in.y / height_half_);
        const T x = (sqrt_3 / 3) * normalized.x - (static_cast<T>(1) / 3) * normalized.y;
        const T y = (static_cast<T>(2) / 3) * normalized.y;
        return hex_round<HexCoordAxialPointy>(HexCoordCubeFractional<T>{x, y, -x - y});
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#hex-to-pixel
    template <typename T>
    Point2d<T> CsHexPointy<T>::to_draw_space(const HexCoordAxialPointy& in) const noexcept
    {
        return
        {
            (static_cast<T>(in.x) + static_cast<T>(in.y) / static_cast<T>(2)) * width_,
            (static_cast<T>(1.5) * static_cast<T>(in.y)) * height_half_
        };
    }
}
