#pragma once

#include "either.hpp"

namespace ge
{
    struct None final
    {
    };

    constexpr None none;

    template <typename T>
    class Option final : private Either<T, None>
    {
    public:
        using Either<T, None>::swap;
        using Either<T, None>::match;

        constexpr Option(T) noexcept;
        constexpr Option(None) noexcept;
        ~Option() override = default;

        constexpr Option(Option&&) noexcept;
        constexpr Option& operator=(Option&&) noexcept;

        constexpr Option(const Option&) = delete;
        constexpr Option& operator=(const Option&) = delete;

        constexpr Option& operator=(None) noexcept;

        constexpr bool is_some() const noexcept;
        constexpr bool is_none() const noexcept;
    };

    template <typename T>
    Option<T> some(T&& v) noexcept;
}

#include "option_impl.hpp"
