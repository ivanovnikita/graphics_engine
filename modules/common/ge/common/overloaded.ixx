export module overloaded;

namespace ge
{
    export template <typename... Ts>
    struct overloaded final : Ts...
    {
        using Ts::operator()...;
    };

    export template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
}
