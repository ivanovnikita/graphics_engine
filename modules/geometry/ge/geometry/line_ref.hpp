#pragma once

#include "point.hpp"

namespace ge
{
    template <typename T>
    struct LineCRef2d final
    {
        const Point2d<T>& a;
        const Point2d<T>& b;
    };

    using LineCRef2dF = LineCRef2d<float>;
    using LineCRef2dD = LineCRef2d<double>;
}
