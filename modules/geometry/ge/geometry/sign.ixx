export module sign;

namespace ge
{
    export template <typename T>
    int sign(T val) noexcept
    {
        return (T(0) < val) - (val < T(0));
    }
}
