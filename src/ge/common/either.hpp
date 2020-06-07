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
        explicit Either(T v) noexcept;
        explicit Either(U v) noexcept;
        ~Either() noexcept;

        Either(Either&&) noexcept;
        Either& operator=(Either&&) noexcept;

        Either(const Either&) = delete;
        Either& operator=(const Either&) = delete;

        void swap(Either&) noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> &&
                std::is_nothrow_invocable_v<SecondF, U&>
        void match(FirstF&& first_func, SecondF&& second_func) noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, const T&> &&
                std::is_nothrow_invocable_v<SecondF, const U&>
        void match(FirstF&& first_func, SecondF&& second_func) const noexcept;

    private:
        enum class either_tag
        {
            first,
            second
        };

        either_tag tag;

        union
        {
            T first;
            U second;
        };
    };
}

#include "either_impl.hpp"
