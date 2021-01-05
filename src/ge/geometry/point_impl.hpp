#pragma once

#include "point.hpp"

namespace ge
{
    template <typename T>
    bool operator==(const Point2d<T>& a, const Point2d<T>& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    template <typename T>
    bool operator!=(const Point2d<T>& a, const Point2d<T>& b)
    {
        return not (a == b);
    }

    template <typename T>
    Point2d<T> operator+(const Point2d<T>& a, const Point2d<T>& b)
    {
        return {a.x + b.x, a.y + b.y};
    }

    template <typename T>
    Point2d<T> operator-(const Point2d<T>& a, const Point2d<T>& b)
    {
        return {a.x - b.x, a.y - b.y};
    }

    template <typename T>
    T cross(const Point2d<T>& a, const Point2d<T>& b)
    {
        return a.x * b.y - b.x * a.y;
    }
}
