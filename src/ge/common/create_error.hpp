#pragma once

#include "errors.hpp"

#include <type_traits>
#include <tuple>

namespace ge
{
    template <typename T, typename... Args>
    using CreateError = typename std::invoke_result_t<decltype(T::create), Args...>::Err;
}

