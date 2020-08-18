#pragma once

#include "concepts.hpp"
#include "copy_from_error.hpp"
#include "result.hpp"

#include <type_traits>
#include <variant>

namespace ge
{
    template <typename... Args>
    class Variant final
    {
    public:
        template <typename T>
            requires
                (... or std::is_nothrow_constructible_v<Args, T>)
        Variant(T) noexcept;

        Variant(Variant&&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>);

        Variant& operator=(Variant&&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>) and
                (... and std::is_nothrow_move_assignable_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        Variant(const Variant&) noexcept
            requires
                (... and std::is_nothrow_copy_constructible_v<Args>);

        Variant& operator=(const Variant&) noexcept
            requires
                (... and std::is_nothrow_copy_constructible_v<Args>) and
                (... and std::is_nothrow_copy_assignable_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        void swap(Variant&) noexcept
            requires
                (... and std::is_nothrow_move_constructible_v<Args>) and
                (... and std::is_nothrow_swappable_v<Args>);

        [[ nodiscard ]] static auto copy_from(const Variant&) noexcept
            -> Result
            <
                Variant,
                CopyFromError<Args...>
            >
            requires
                (... and std::is_nothrow_move_constructible_v<Args>) and
                (... and (CopyableFrom<Args> or std::is_nothrow_copy_constructible_v<Args>));

        template <typename... Fs>
        decltype(auto) match(Fs&&...) noexcept;

    private:
        std::variant<Args...> storage;
    };
}

#include "variant_impl.hpp"
