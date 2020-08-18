#pragma once

#include "construct.hpp"

#include <memory>

namespace ge
{
    template <typename T, typename... Args>
        requires std::is_nothrow_constructible_v<T, Args...>
    void construct_at(void* ptr, Args&&... args) noexcept
    {
        std::construct_at(reinterpret_cast<T*>(ptr), std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
        requires
            Creatable<T, Args...> and
            std::is_nothrow_move_constructible_v<T>
    [[ nodiscard ]] auto create_at(void* ptr, Args&&... args) noexcept
        -> Option<CreateError<T, Args...>>
    {
        using Errors = CreateError<T, Args...>;

        return T::create(std::forward<Args>(args)...)
            .match
            (
                [ptr] (T&& value) noexcept -> Option<Errors>
                {
                    std::construct_at(reinterpret_cast<T*>(ptr), std::move(value));
                    return none;
                },
                [] (Errors&& errors) noexcept -> Option<Errors>
                {
                    return std::move(errors);
                }
            );
    }
}

