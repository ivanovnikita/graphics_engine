#pragma once

#include "safe_cast.hpp"
#include "exception.h"

#include <limits>

namespace ge
{

    template<typename To, typename From>
    constexpr bool is_both_unsigned()
    {
        return std::is_unsigned_v<To> && std::is_unsigned_v<From>;
    }

    template<typename To, typename From>
    constexpr bool is_both_same_sign()
    {
        return std::is_signed_v<To> == std::is_signed_v<From>;
    }

    template<typename To, typename From>
    To safe_cast(const From& from)
    {
        static_assert(std::is_integral<From>::value && std::is_integral<To>::value, "Types must be integral");

        static constexpr auto toMin = static_cast<From>(std::numeric_limits<To>::min());
        static constexpr auto toMax = static_cast<From>(std::numeric_limits<To>::max());
        static constexpr auto zero = static_cast<From>(0);

        if constexpr
        (
            (is_both_same_sign<To, From>() && sizeof(From) <= sizeof(To))
            || (std::is_signed<To>::value && sizeof(From) < sizeof(To))
        )
        {
            return static_cast<To>(from);
        }
        else if constexpr
        (
            (std::is_unsigned_v<From> && std::is_signed_v<To> && sizeof(From) >= sizeof(To))
            || (is_both_unsigned<From, To>() && sizeof(From) > sizeof(To))
        )
        {
            if (from > toMax)
            {
                GE_THROW(overflow_error, "");
            }
        }
        else if constexpr(std::is_signed_v<From> && std::is_unsigned_v<To> && sizeof(From) <= sizeof(To))
        {
            if (from < zero)
            {
                GE_THROW(overflow_error, "");
            }
        }
        else if constexpr (std::is_unsigned_v<To>)
        {
            if (from > toMax || from < zero)
            {
                GE_THROW(overflow_error, "");
            }
        }
        else
        {
            if (from > toMax || from < toMin)
            {
                GE_THROW(overflow_error, "");
            }
        }

        return static_cast<To>(from);
    }

}
