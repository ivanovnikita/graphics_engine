#pragma once

#include "concepts.hpp"
#include "copy_from_error.hpp"
#include "result.hpp"

#include <type_traits>
#include <concepts>
#include <functional>
#include <string_view>

namespace ge
{
    template <typename T, typename U>
    class Either final
    {
    public:
        constexpr Either(T v) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        constexpr Either(U v) noexcept
            requires
                std::is_nothrow_move_constructible_v<U>;

        constexpr ~Either() noexcept
            requires
                std::is_nothrow_destructible_v<T> and
                std::is_nothrow_destructible_v<U>;

        constexpr Either(Either&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<U>;

        constexpr Either& operator=(Either&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<U> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<U>;

        constexpr Either(const Either&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T> and
                std::is_nothrow_copy_constructible_v<U>;

        constexpr Either& operator=(const Either&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T> and
                std::is_nothrow_copy_constructible_v<U> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<U>;

        constexpr void swap(Either&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<U> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<U>;

        [[ nodiscard ]] static constexpr auto copy_from(const Either&) noexcept
            -> Result
            <
                Either,
                CopyFromError<T, U>
            >
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<U> and
                (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>) and
                (CopyableFrom<U> or std::is_nothrow_copy_constructible_v<U>);

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> and
                std::is_nothrow_invocable_v<SecondF, U&>
        constexpr auto match(FirstF&& first_func, SecondF&& second_func) noexcept
            -> std::common_type_t<std::invoke_result_t<FirstF, T&>, std::invoke_result_t<SecondF, U&>>;

        template <typename FirstF, typename SecondF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> and
                std::is_nothrow_invocable_v<SecondF, U&>
        constexpr auto match(FirstF&& first_func, SecondF&& second_func) const noexcept
            -> std::common_type_t<std::invoke_result_t<FirstF, const T&>, std::invoke_result_t<SecondF, const U&>>;


        template <typename FirstF>
            requires
                std::is_nothrow_invocable_v<FirstF, T&> and
                std::is_same_v<std::invoke_result_t<FirstF, T&>, void>
        constexpr void match_first(FirstF&& first_func) noexcept;

        template <typename FirstF>
            requires
                std::is_nothrow_invocable_v<FirstF, const T&> and
                std::is_same_v<std::invoke_result_t<FirstF, const T&>, void>
        constexpr void match_first(FirstF&& first_func) const noexcept;

        template <typename SecondF>
            requires
                std::is_nothrow_invocable_v<SecondF, U&> and
                std::is_same_v<std::invoke_result_t<SecondF, U&>, void>
        constexpr void match_second(SecondF&& second_func) noexcept;

        template <typename SecondF>
            requires
                std::is_nothrow_invocable_v<SecondF, const U&> and
                std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>
        constexpr void match_second(SecondF&& second_func) const noexcept;

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
