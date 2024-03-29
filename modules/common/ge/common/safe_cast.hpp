#pragma once

namespace ge
{
    template<typename To, typename From>
    constexpr To safe_cast(const From& from);
}

#include "ge/common/safe_cast_impl.hpp"
