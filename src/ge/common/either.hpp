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
    class Either
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
                std::is_nothrow_invocable_v<SecondF, U&> &&
                std::is_same_v<std::invoke_result_t<FirstF, T&>, void> &&
                std::is_same_v<std::invoke_result_t<SecondF, U&>, void>
        constexpr void match(FirstF&& first_func, SecondF&& second_func) noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, const T&> &&
                std::is_nothrow_invocable_v<SecondF, const U&> &&
                std::is_same_v<std::invoke_result_t<FirstF, const T&>, void> &&
                std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>
        constexpr void match(FirstF&& first_func, SecondF&& second_func) const noexcept;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> &&
                std::is_nothrow_invocable_v<SecondF, U&> &&
                (not std::is_same_v<std::invoke_result_t<FirstF, T&>, void>) &&
                (not std::is_same_v<std::invoke_result_t<SecondF, U&>, void>)
        constexpr auto match(FirstF&& first_func, SecondF&& second_func) noexcept
            -> std::common_type_t<std::invoke_result_t<FirstF, T&>, std::invoke_result_t<SecondF, U&>>;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> &&
                std::is_nothrow_invocable_v<SecondF, U&> &&
                (not std::is_same_v<std::invoke_result_t<FirstF, const T&>, void>) &&
                (not std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>)
        constexpr auto match(FirstF&& first_func, SecondF&& second_func) const noexcept
            -> std::common_type_t<std::invoke_result_t<FirstF, const T&>, std::invoke_result_t<SecondF, const U&>>;

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
