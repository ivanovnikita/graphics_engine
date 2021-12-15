#pragma once

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
    concept Loggable =
        std::integral<T> or
        std::floating_point<T> or
        ConvertibleToStringView<T> or
        std::ranges::range<T>;

    void log(LogDestination, Loggable auto ...) noexcept;

    void log_flush(LogDestination) noexcept;
}

#include "log_impl.hpp"
