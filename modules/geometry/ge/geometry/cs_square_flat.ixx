module;

#include <cmath>

export module cs_square_flat;

export import coord_helpers;
import point;
import sign;
import point_localization;

namespace ge
{
    export struct SquareCoordAxialFlat final
    {
        int x;
        int y;
    };

    export template <typename T>
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

    export template <typename T>
    CsSquareFlat(T, T, T) -> CsSquareFlat<T>;
}

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
        const T x = static_cast<T>(in.x) * width_ + static_cast<T>(in.y) * x_1_;

        return {x, y};
    }
}
