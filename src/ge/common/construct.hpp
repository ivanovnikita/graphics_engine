#pragma once

#include "create_error.hpp"
#include "option.hpp"
#include "result.hpp"

#include <type_traits>

namespace ge
{
    template <typename T, typename... Args>
        requires
            std::is_nothrow_constructible_v<T, Args...>
    void construct_at(void*, Args&&...) noexcept;

    template <typename T, typename... Args>
        requires
            Creatable<T, Args...> and
            std::is_nothrow_move_constructible_v<T>
    [[ nodiscard ]] auto create_at(void*, Args&&...) noexcept
        -> Option<CreateError<T, Args...>>;
}

#include "construct_impl.hpp"
