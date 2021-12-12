#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <string_view>

#include <cstdio>

namespace ge
{
    enum class LogDestination
    {
        StdOut,
        StdErr
    };

    void log(LogDestination, std::string_view) noexcept;
    void log(LogDestination, char*) noexcept;
    void log(LogDestination, const char*) noexcept;
    void log_flush(LogDestination) noexcept;

    void log(LogDestination, std::integral auto) noexcept;
    void log(LogDestination, std::floating_point auto) noexcept;

    template <typename T>
    concept Loggable =
        std::integral<T> or
        std::floating_point<T> or
        std::convertible_to<T, std::string_view>;
//        std::is_same_v<std::remove_cvref_t<T>, std::string_view> or
//        std::is_same_v<std::remove_cvref_t<T>, std::string> or
//        std::is_same_v<std::remove_extent_t<std::remove_cvref_t<T>>, char*> or
//        std::is_same_v<std::remove_extent_t<std::remove_cvref_t<T>>, const char*>;

    void log(LogDestination, Loggable auto ...) noexcept;
}

#include "log_impl.hpp"
