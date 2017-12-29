#pragma once

namespace ge
{

    template<typename To, typename From>
    To safe_cast(const From& from);

}

#include "safe_cast_impl.hpp"
