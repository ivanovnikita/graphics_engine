#pragma once

#include "flags.hpp"

#include <cassert>

namespace ge
{
    template <typename EnumFlags>
    template <typename... Fs>
    constexpr Flags<EnumFlags>::Flags(Fs... flags) noexcept
    {
        (set(flags), ...);
    }

    template <typename EnumFlags>
    constexpr void Flags<EnumFlags>::set(EnumFlags flag) noexcept
    {
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) >= 0);
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) < 8 * static_cast<std::underlying_type_t<EnumFlags>>(sizeof(EnumFlags)));
        bits_.set(static_cast<size_t>(flag));
    }

    template <typename EnumFlags>
    constexpr void Flags<EnumFlags>::reset(EnumFlags flag) noexcept
    {
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) >= 0);
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) < 8 * static_cast<std::underlying_type_t<EnumFlags>>(sizeof(EnumFlags)));
        bits_.reset(static_cast<size_t>(flag));
    }

    template <typename EnumFlags>
    constexpr bool Flags<EnumFlags>::test(EnumFlags flag) const noexcept
    {
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) >= 0);
        assert(static_cast<std::underlying_type_t<EnumFlags>>(flag) < 8 * static_cast<std::underlying_type_t<EnumFlags>>(sizeof(EnumFlags)));
        return bits_.test(static_cast<size_t>(flag));
    }
}
