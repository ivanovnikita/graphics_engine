#pragma once

namespace ge
{
    template <typename T>
    struct Point2d final
    {
        T x;
        T y;
    };

    using Point2dF = Point2d<float>;
    using Point2dD = Point2d<double>;

    template <typename T>
    bool operator==(const Point2d<T>&, const Point2d<T>&) noexcept;

    template <typename T>
    bool operator!=(const Point2d<T>&, const Point2d<T>&) noexcept;

    template <typename T>
    Point2d<T> operator+(const Point2d<T>&, const Point2d<T>&) noexcept;

    template <typename T>
    Point2d<T> operator-(const Point2d<T>&, const Point2d<T>&) noexcept;

    template <typename T>
    T cross(const Point2d<T>&, const Point2d<T>&) noexcept;
}

#include "point_impl.hpp"
