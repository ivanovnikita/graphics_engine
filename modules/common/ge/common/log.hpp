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
    concept Loggable =
        std::integral<T> or
        std::floating_point<T> or
        std::convertible_to<T, std::string_view> or
        std::ranges::range<T>;

    void log(LogDestination, Loggable auto ...) noexcept;

    void log_flush(LogDestination) noexcept;
}

#include "log_impl.hpp"
