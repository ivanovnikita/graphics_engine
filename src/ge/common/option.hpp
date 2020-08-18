#pragma once

#include "concepts.hpp"
#include "copy_from_error.hpp"
#include "result.hpp"

#include <type_traits>
#include <string_view>

namespace ge
{
    struct None final {};

    constexpr None none;

    template <typename T>
    class Option final
    {
    public:
        constexpr Option(T) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        constexpr Option(None) noexcept;

        constexpr Option(Result<Ok, T>) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        constexpr ~Option() noexcept
            requires
                std::is_nothrow_destructible_v<T>;

        constexpr Option(Option&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        constexpr Option& operator=(Option&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_swappable_v<T>;

        constexpr Option(const Option&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T>;

        constexpr Option& operator=(const Option&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T> and
                std::is_nothrow_swappable_v<T>;

        constexpr Option& operator=(None) noexcept;

        constexpr void swap(Option&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_swappable_v<T>;

        [[ nodiscard ]] static constexpr auto copy_from(const Option&) noexcept
            -> Result
            <
                Option,
                CopyFromError<T>
            >
            requires
                std::is_nothrow_move_constructible_v<T> and
                (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>);

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> and
                std::is_nothrow_invocable_v<NoneF>
        constexpr auto match(SomeF&& some_func, NoneF&& none_func) & noexcept
            -> std::common_type_t<std::invoke_result_t<SomeF, T&>, std::invoke_result_t<NoneF>>;

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> and
                std::is_nothrow_invocable_v<NoneF>
        constexpr auto match(SomeF&& some_func, NoneF&& none_func) && noexcept
            -> std::common_type_t<std::invoke_result_t<SomeF, T&&>, std::invoke_result_t<NoneF>>;

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, const T&> and
                std::is_nothrow_invocable_v<NoneF>
        constexpr auto match(SomeF&& some_func, NoneF&& none_func) const noexcept
            -> std::common_type_t<std::invoke_result_t<SomeF, const T&>, std::invoke_result_t<NoneF>>;

        template <typename SomeF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> and
                std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
        constexpr void match_some(SomeF&& some_func) & noexcept;

        template <typename SomeF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> and
                std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
        constexpr void match_some(SomeF&& some_func) && noexcept;

        template <typename SomeF>
            requires
                std::is_nothrow_invocable_v<SomeF, const T&> and
                std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>
        constexpr void match_some(SomeF&& some_func) const noexcept;

        template <typename NoneF>
            requires
                std::is_nothrow_invocable_v<NoneF> and
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match_none(NoneF&& none_func) noexcept;

        template <typename NoneF>
            requires
                std::is_nothrow_invocable_v<NoneF> and
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match_none(NoneF&& none_func) const noexcept;

        constexpr bool is_some() const noexcept;
        constexpr bool is_none() const noexcept;

        template <typename ThenF>
            requires
                std::is_nothrow_invocable_v<ThenF, T&&> and
                (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>) and
                (not Optionable<std::invoke_result_t<ThenF, T&&>>)
        constexpr auto then(ThenF&& then_func) && noexcept
            -> Option<std::invoke_result_t<ThenF, T&&>>;

        template <typename ThenF>
            requires
                std::is_nothrow_invocable_v<ThenF, T&&> and
                (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>) and
                Optionable<std::invoke_result_t<ThenF, T&&>>
        constexpr auto then(ThenF&& then_func) && noexcept
            -> Option<std::invoke_result_t<ThenF, T&&>>;

    private:
        bool contains_value;

        struct some_tag final {};
        struct none_tag final {};

        union Storage
        {
            constexpr Storage(some_tag, T v) noexcept;
            constexpr Storage(none_tag) noexcept;
            constexpr ~Storage() noexcept;

            T some;
            None none;
        } storage;
    };

    template <typename T>
    Option<T> some(T v) noexcept;
}

#include "option_impl.hpp"
