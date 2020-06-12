#pragma once

#include "either.hpp"

namespace ge
{
    struct None final
    {
    };

    constexpr None none;

    template <typename T>
    class Option final : private Either<T, None>
    {
    public:
        using Either<T, None>::swap;

        constexpr Option(T) noexcept;
        constexpr Option(None) noexcept;
        ~Option() override = default;

        constexpr Option(Option&&) noexcept;
        constexpr Option& operator=(Option&&) noexcept;

        constexpr Option(const Option&) = delete;
        constexpr Option& operator=(const Option&) = delete;

        constexpr Option& operator=(None) noexcept;

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> &&
                std::is_nothrow_invocable_v<NoneF> &&
                std::is_same_v<std::invoke_result_t<SomeF, T&>, void> &&
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match(SomeF&& some_func, NoneF&& none_func) noexcept;

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, const T&> &&
                std::is_nothrow_invocable_v<NoneF> &&
                std::is_same_v<std::invoke_result_t<SomeF, const T&>, void> &&
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match(SomeF&& some_func, NoneF&& none_func) const noexcept;


        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> &&
                std::is_nothrow_invocable_v<NoneF> &&
                (not std::is_same_v<std::invoke_result_t<SomeF, T&>, void>) &&
                (not std::is_same_v<std::invoke_result_t<NoneF>, void>)
        constexpr auto match(SomeF&& some_func, NoneF&& none_func) noexcept
            -> std::common_type_t<std::invoke_result_t<SomeF, T&>, std::invoke_result_t<NoneF>>;

        template <typename SomeF, typename NoneF>
            requires
                std::is_nothrow_invocable_v<SomeF, const T&> &&
                std::is_nothrow_invocable_v<NoneF> &&
                (not std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>) &&
                (not std::is_same_v<std::invoke_result_t<NoneF>, void>)
        constexpr auto match(SomeF&& some_func, NoneF&& none_func) const noexcept
            -> std::common_type_t<std::invoke_result_t<SomeF, const T&>, std::invoke_result_t<NoneF>>;

        template <typename SomeF>
            requires
                std::is_nothrow_invocable_v<SomeF, T&> &&
                std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
        constexpr void match_some(SomeF&& some_func) noexcept;

        template <typename SomeF>
            requires
                std::is_nothrow_invocable_v<SomeF, const T&> &&
                std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>
        constexpr void match_some(SomeF&& some_func) const noexcept;

        template <typename NoneF>
            requires
                std::is_nothrow_invocable_v<NoneF> &&
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match_none(NoneF&& none_func) noexcept;

        template <typename NoneF>
            requires
                std::is_nothrow_invocable_v<NoneF> &&
                std::is_same_v<std::invoke_result_t<NoneF>, void>
        constexpr void match_none(NoneF&& none_func) const noexcept;

        constexpr bool is_some() const noexcept;
        constexpr bool is_none() const noexcept;
    };

    template <typename T>
    Option<T> some(T v) noexcept;
}

#include "option_impl.hpp"
