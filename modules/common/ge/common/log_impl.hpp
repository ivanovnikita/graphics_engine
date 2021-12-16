#pragma once

#include "log.hpp"

#include <array>
#include <charconv>

#include <cassert>

namespace ge
{
    namespace detail
    {
        void log(LogDestination, std::string_view) noexcept;

        template <typename T>
            requires ConvertibleToStringView<T>
        void log(const LogDestination destination, const T& value) noexcept
        {
            if constexpr (std::convertible_to<T, std::string_view>)
            {
                log(destination, std::string_view{value});
            }
            else
            {
                log(destination, to_string_view(value));
            }
        }

        void log(const LogDestination destination, std::integral auto value) noexcept
        {
            // separate func 'void log(LogDestination, bool)' is useless:
            // cases like "log(..., "some log message")" calls overload with 'bool' param
            // instead of 'std::string_view' param
            if constexpr (std::is_same_v<decltype(value), bool>)
            {
                log(destination, std::string_view{value ? "true" : "false"});
            }
            else
            {
                // uint64_t max: 18 446 744 073 709 551 615
                // int64_t min:  -9 223 372 036 854 775 808
                constexpr size_t MAX_INTEGER_STRING_LEN = 21;
                std::array<char, MAX_INTEGER_STRING_LEN> buffer;
                const std::to_chars_result result = std::to_chars
                (
                    buffer.data(),
                    buffer.data() + buffer.size(),
                    value
                );
                if (result.ec == std::errc())
                {
                    log(destination, std::string_view{buffer.data(), result.ptr});
                }
                else
                {
                    log(destination, "std::to_chars<integral> failed, log may be invalid: ");
                    log(destination, std::string_view{buffer.data(), buffer.size()});
                }
            }
        }

        void log(const LogDestination destination, std::floating_point auto value) noexcept
        {
            constexpr size_t MAX_FLOATING_POINT_STRING_LEN = 64;
            std::array<char, MAX_FLOATING_POINT_STRING_LEN> buffer;
            const std::to_chars_result result = std::to_chars
            (
                buffer.data(),
                buffer.data() + buffer.size(),
                value
            );
            if (result.ec == std::errc())
            {
                log(destination, std::string_view{buffer.data(), result.ptr});
            }
            else
            {
                log(destination, "std::to_chars<floating_point> failed, log may be invalid: ");
                log(destination, std::string_view{buffer.data(), buffer.size()});
            }
        }

        template <typename T>
            requires NonTrivialLoggable<T>
        void log(const LogDestination destination, const T& value) noexcept
        {
            log_non_trivial(destination, value);
        }

        template <typename T>
            requires std::ranges::range<T> and
                (not std::convertible_to<T, std::string_view>)
        void log(const LogDestination destination, T&& range) noexcept
        {
            log(destination, "[");

            auto it = std::ranges::begin(range);
            const auto it_end = std::ranges::end(range);
            if (it != it_end)
            {
                log(destination, *it);
                ++it;
            }

            while (it != it_end)
            {
                log(destination, ", ", *it);
                ++it;
            }

            log(destination, "]");
        }
    }

    void log(const LogDestination destination, Loggable auto ... values) noexcept
    {
        (detail::log(destination, values), ...);
    }
}
