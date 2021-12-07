#pragma once

#include <bitset>

namespace ge
{
    template <typename EnumFlags>
    class Flags final
    {
    public:
        template <typename... Fs>
        constexpr explicit Flags(Fs...) noexcept;

        constexpr explicit Flags() noexcept;

        constexpr void set(EnumFlags flag) noexcept;

        constexpr void reset(EnumFlags flag) noexcept;

        constexpr bool test(EnumFlags flag) const noexcept;

    private:
        std::bitset<8 * sizeof(EnumFlags)> bits_;
    };
}

#include "flags_impl.hpp"
