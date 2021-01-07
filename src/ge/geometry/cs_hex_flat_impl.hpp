#pragma once

#include "cs_hex_flat.hpp"
#include "hex_coord_cube_fractional.hpp"
#include "point_localization.hpp"
#include "sign.hpp"

#include <cmath>
#include <cstdlib>

namespace ge
{
    template <typename T>
        requires Coord2dLike<T>
    bool operator==(const T& a, const T& b) noexcept
    {
        return a.x == b.x and a.y == b.y;
    }

    template <typename T>
        requires Coord2dLike<T>
    bool operator!=(const T& a, const T& b) noexcept
    {
        return not (a == b);
    }

    template <typename T>
        requires Coord2dLike<T>
    std::ostream& operator<<(std::ostream& out, const T& v)
    {
        out << "{" << v.x << ", " << v.y << "}";
        return out;
    }

    template <typename T>
    CsHexFlat<T>::CsHexFlat
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
        , last_x_{width_ + x_2_}
        , pre_last_x_{width_half_ + (x_2_ - x_1_)}
    {
    }

    template <typename T>
    HexCoordDoubledHeight CsHexFlat<T>::to_hex_doubled_height(const Point2d<T>& in) const noexcept
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
                    const Point2d<T> b{last_x_, height_half_};
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
                    const Point2d<T> a{last_x_, height_half_};
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
    Point2d<T> CsHexFlat<T>::to_draw_space(const HexCoordDoubledHeight& in) const noexcept
    {
        T x = sign(in.x) * (std::abs(in.x) / 2) * width_cycle_;
        T y = sign(in.y) * (std::abs(in.y) / 2) * height_;

        if ((in.x & 1) == 1)
        {
            x += sign(in.x) * (0.5 * width_ + x_2_);
            y += sign(in.y) * 0.5 * height_;
        }

        return {x, y};
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#pixel-to-hex
    template <typename T>
    HexCoordAxialFlat CsHexFlat<T>::to_hex_axial_flat(const Point2d<T>& in) const noexcept
    {
        const Point2d<T> normalized(in.x / width_half_, std::sqrt(3) * (in.y / height_));
        const T x = (static_cast<T>(2) / 3) * normalized.x;
        const T y = (static_cast<T>(-1) / 3) * normalized.x + (std::sqrt(static_cast<T>(3)) / 3) * normalized.y;
        return hex_round<HexCoordAxialFlat>(HexCoordCubeFractional<T>{x, y, -x - y});
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#hex-to-pixel
    template <typename T>
    Point2d<T> CsHexFlat<T>::to_draw_space(const HexCoordAxialFlat& in) const noexcept
    {
        const T sqrt_3 = std::sqrt(static_cast<T>(3));
        return
        {
            ((static_cast<T>(3) / 2) * in.x) * width_half_,
            ((sqrt_3 / 2) * in.x + sqrt_3 * in.y) * (height_ / sqrt_3)
        };
    }
}
