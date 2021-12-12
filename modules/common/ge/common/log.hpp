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

    void log(LogDestination, std::string_view) noexcept;

    template <typename T>
        requires std::convertible_to<T, std::string_view>
    void log(LogDestination, const T&) noexcept;

    void log_flush(LogDestination) noexcept;

    void log(LogDestination, std::integral auto) noexcept;
    void log(LogDestination, std::floating_point auto) noexcept;

    template <typename T>
        requires std::ranges::range<T> and
            (not std::convertible_to<T, std::string_view>)
    void log(LogDestination, const T&) noexcept;

    template <typename T>
    concept Loggable =
        std::integral<T> or
        std::floating_point<T> or
        std::convertible_to<T, std::string_view> or
        std::ranges::range<T>;

    void log(LogDestination, Loggable auto ...) noexcept;
}

#include "log_impl.hpp"
