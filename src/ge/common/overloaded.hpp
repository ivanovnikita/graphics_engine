#pragma once

namespace ge
{
    template <typename... Ts>
    struct overloaded final : Ts...
    {
        using Ts::operator()...;
    };

    template <typename... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
}
