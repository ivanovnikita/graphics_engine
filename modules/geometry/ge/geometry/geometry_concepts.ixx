export module geometry.concepts;

namespace ge
{
    export template <typename T>
    concept Coord2dLike = requires
    (T c)
    {
        c.x;
        c.y;
    };
}
