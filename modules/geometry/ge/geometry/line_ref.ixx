export module line_ref;

export import point;

namespace ge
{
    export template <typename T>
    struct LineCRef2d final
    {
        const Point2d<T>& a;
        const Point2d<T>& b;
    };

    export using LineCRef2dF = LineCRef2d<float>;
    export using LineCRef2dD = LineCRef2d<double>;
}
