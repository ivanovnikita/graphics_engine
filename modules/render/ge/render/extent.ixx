export module extent;

namespace ge
{
    export template <typename T>
    struct Extent final
    {
        T width;
        T height;
    };
}
