#pragma once

#include "reflection/reflection.hpp"

#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <ranges>

#include <cstdio>

namespace ge
{
    enum class LogDestination
    {
        StdOut,
        StdErr
    };

    template <typename T>
    concept ConvertibleToStringView =
        std::convertible_to<T, std::string_view> or
        requires(T t)
        {
            { to_string_view(t) } -> std::same_as<std::string_view>;
        };

    template <typename T>
    concept NonTrivialLoggable = requires(T t, LogDestination destination)
    {
        log_non_trivial(destination, t);
    };

    // TODO: log unions

    template <typename T>
    concept Loggable =
        std::integral<T> or
        std::floating_point<T> or
        ConvertibleToStringView<T> or
        NonTrivialLoggable<T> or
        RegisteredMembers<T> or
        std::ranges::range<T>;

    template <typename T>
        requires (std::is_pointer_v<T> and not ConvertibleToStringView<T>)
    void log_non_trivial(LogDestination, T) noexcept;

    void log(LogDestination, Loggable auto ...) noexcept;

    void log_flush(LogDestination) noexcept;
}

#include "log_impl.hpp"
