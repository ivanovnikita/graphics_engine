module;

#include <cmath>
#include <cstdint>
#include <cstdlib>

export module cs_hex_flat;

export import coord_helpers;
import point;
import line_ref;
import sign;
import point_localization;
import hex_coord_cube_fractional;

namespace ge
{
    // doubled height coords: https://www.redblobgames.com/grids/hexagons/#coordinates-doubled
    export struct HexCoordDoubledHeight final
    {
        int x;
        int y;
    };

    // offset odd-q coords in vertical layout: https://www.redblobgames.com/grids/hexagons/#coordinates-offset
    export struct HexCoordOffsetFlat final
    {
        int x;
        int y;
    };

    export struct HexCoordAxialFlat final
    {
        int x;
        int y;

        int z() const noexcept;
    };

    export HexCoordOffsetFlat to_hex_offset_flat(const HexCoordDoubledHeight&) noexcept;
    export HexCoordOffsetFlat to_hex_offset_flat(const HexCoordAxialFlat&) noexcept;

    export HexCoordDoubledHeight to_hex_doubled_height(const HexCoordOffsetFlat&) noexcept;
    export HexCoordDoubledHeight to_hex_doubled_height(const HexCoordAxialFlat&) noexcept;

    export HexCoordAxialFlat to_hex_axial_flat(const HexCoordDoubledHeight&) noexcept;
    export HexCoordAxialFlat to_hex_axial_flat(const HexCoordOffsetFlat&) noexcept;

    // Opposite sides of hex are parallel and symmetric
    // Origin of draw space CS is in the center of hex (0; 0)
    export template <typename T>
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
        Point2d<T> to_draw_space(const HexCoordDoubledHeight&) const noexcept; // hex center

        HexCoordAxialFlat to_hex_axial_flat(const Point2d<T>&) const noexcept;
        Point2d<T> to_draw_space(const HexCoordAxialFlat&) const noexcept; // hex center

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

    export template <typename T>
    CsHexFlat(T, T, T, T) -> CsHexFlat<T>;
}

namespace ge
{
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
        const T x_rem = std::fabs(std::fmod(in.x, width_cycle_));

        const int y_quot = static_cast<int>(in.y / height_);
        const T y_rem = std::fabs(std::fmod(in.y, height_));

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
                else
                {
                    const Point2d<T> a{last_x_, height_half_};
                    const Point2d<T> b{pre_last_x_, height_};
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
            if (y_rem < height_half_)
            {
                const Point2d<T> a{width_half_, 0};
                const Point2d<T> b{x_2_, height_half_};
                if (not is_left_side(LineCRef2d<T>{a, b}, {x_rem, y_rem}))
                {
                    x += 1 * sign(in.x);
                    y += 1 * sign(in.y);
                }
            }
            else
            {
                const Point2d<T> a{x_2_, height_half_};
                const Point2d<T> b{width_half_, height_};
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
        }

        return {x, y};
    }

    template <typename T>
    Point2d<T> CsHexFlat<T>::to_draw_space(const HexCoordDoubledHeight& in) const noexcept
    {
        T x = (static_cast<T>(in.x) / 2.f) * width_cycle_;
        T y = (static_cast<T>(in.y) / 2.f) * height_;

        return {x, y};
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#pixel-to-hex
    template <typename T>
    HexCoordAxialFlat CsHexFlat<T>::to_hex_axial_flat(const Point2d<T>& in) const noexcept
    {
        const T sqrt_3 = std::sqrt(static_cast<T>(3));
        const Point2d<T> normalized{in.x / width_half_, sqrt_3 * (in.y / height_)};
        const T x = (static_cast<T>(2) / 3) * normalized.x;
        const T y = (static_cast<T>(-1) / 3) * normalized.x + (sqrt_3 / 3) * normalized.y;
        return hex_round<HexCoordAxialFlat>(HexCoordCubeFractional<T>{x, y, -x - y});
    }

    // https://www.redblobgames.com/grids/hexagons/implementation.html#hex-to-pixel
    template <typename T>
    Point2d<T> CsHexFlat<T>::to_draw_space(const HexCoordAxialFlat& in) const noexcept
    {
        return
        {
            (static_cast<T>(1.5) * static_cast<T>(in.x)) * width_half_,
            (static_cast<T>(in.x) / static_cast<T>(2) + static_cast<T>(in.y)) * height_
        };
    }
}
