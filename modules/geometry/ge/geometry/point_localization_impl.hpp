#pragma once

#include "point_localization.hpp"

namespace ge
{
    // TODO: implement more precise method
    template <typename T>
    LineSide line_side(const LineCRef2d<T>& l, const Point2d<T>& p)
    {
        const T det = cross(l.b - l.a, p - l.a);
        if (det > 0)
        {
            return LineSide::Left;
        }
        else if (det < 0)
        {
            return LineSide::Right;
        }

        return LineSide::Collinear;
    }

    template <typename T>
    bool is_left_side(const LineCRef2d<T>& l, const Point2d<T>& p)
    {
        return cross(l.b - l.a, p - l.a) > 0;
    }
}
