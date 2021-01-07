#pragma once

#include "point.hpp"

#include <cmath>

namespace ge
{
    template <typename T>
    bool operator==(const Point2d<T>& a, const Point2d<T>& b) noexcept
    {
        constexpr T eps = 10 * std::numeric_limits<T>::epsilon();
        return (std::abs(a.x - b.x) <= eps) and (std::abs(a.y - b.y) <= eps);
    }

    template <typename T>
    bool operator!=(const Point2d<T>& a, const Point2d<T>& b) noexcept
    {
        return not (a == b);
    }

    template <typename T>
    Point2d<T> operator+(const Point2d<T>& a, const Point2d<T>& b) noexcept
    {
        return {a.x + b.x, a.y + b.y};
    }

    template <typename T>
    Point2d<T> operator-(const Point2d<T>& a, const Point2d<T>& b) noexcept
    {
        return {a.x - b.x, a.y - b.y};
    }

    template <typename T>
    T cross(const Point2d<T>& a, const Point2d<T>& b) noexcept
    {
        return a.x * b.y - b.x * a.y;
    }
}
