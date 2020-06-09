#pragma once

#include <type_traits>
#include <concepts>
#include <functional>

namespace ge
{
    template <typename T, typename U>
        requires
            std::is_nothrow_move_constructible_v<T> &&
            std::is_nothrow_move_constructible_v<U> &&
            std::is_nothrow_move_assignable_v<T> &&
            std::is_nothrow_move_assignable_v<U> &&
            std::is_nothrow_destructible_v<T> &&
            std::is_nothrow_destructible_v<U> &&
            std::is_nothrow_swappable_v<T> &&
            std::is_nothrow_swappable_v<U>
    class Either final
    {
    public:
        explicit constexpr Either(T v) noexcept;
        explicit constexpr Either(U v) noexcept;
        constexpr virtual ~Either() noexcept;

        constexpr Either(Either&&) noexcept;
        constexpr Either& operator=(Either&&) noexcept;

        constexpr Either(const Either&) = delete;
        constexpr Either& operator=(const Either&) = delete;

        constexpr void swap(Either&) noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> &&
                std::is_nothrow_invocable_v<SecondF, U&>
        constexpr void match(FirstF&& first_func, SecondF&& second_func) noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, const T&> &&
                std::is_nothrow_invocable_v<SecondF, const U&>
        constexpr void match(FirstF&& first_func, SecondF&& second_func) const noexcept;

        constexpr bool is_first() const noexcept;
        constexpr bool is_second() const noexcept;

    private:
        enum class either_tag
        {
            first,
            second
        };

        struct first_tag final {};
        struct second_tag final {};

        either_tag tag;

        union Storage
        {
            constexpr Storage(first_tag, T v) noexcept;
            constexpr Storage(second_tag, U v) noexcept;
            constexpr ~Storage() noexcept;

            T first;
            U second;
        } storage;
    };
}

#include "either_impl.hpp"
