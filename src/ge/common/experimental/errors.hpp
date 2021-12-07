#pragma once

#include "concepts.hpp"

#include <type_traits>
#include <variant>

namespace ge
{
    template <typename... Args>
    class Errors final
    {
    public:
        template <typename T>
            requires
                (... or std::is_nothrow_constructible_v<Args, T>)
        Errors(T) noexcept;

        template <typename... SubArgs>
            requires
                (... and std::is_nothrow_constructible_v<std::variant<Args...>, SubArgs>)
        Errors(Errors<SubArgs...>&&) noexcept;

        Errors(Errors&&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>);

        Errors& operator=(Errors&&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>) and
                (... and std::is_nothrow_move_assignable_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        Errors(const Errors&) noexcept
            requires
                (... and std::is_nothrow_copy_constructible_v<Args>);

        Errors& operator=(const Errors&) noexcept
            requires
                (... and std::is_nothrow_copy_constructible_v<Args>) and
                (... and std::is_nothrow_copy_assignable_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        void swap(Errors&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        template <typename... Fs>
        decltype(auto) match(Fs&&...) noexcept;

    private:
        std::variant<Args...> storage;
    };
}

#include "errors_impl.hpp"
