#pragma once

#include "option.hpp"

namespace ge
{
    template <typename T>
    constexpr Option<T>::Option(T value) noexcept
        : Either<T, None>(std::move(value))
    {
    }

    template <typename T>
    constexpr Option<T>::Option(None) noexcept
        : Either<T, None>(none)
    {
    }

    template <typename T>
    constexpr Option<T>::Option(Option&& other) noexcept
        : Either<T, None>(std::move(other))
    {
        if (other.is_some())
        {
            other = none;
        }
    }

    template <typename T>
    constexpr Option<T>& Option<T>::operator=(Option&& other) noexcept
    {
        if (this != &other)
        {
            Option(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename T>
    constexpr Option<T>& Option<T>::operator=(None) noexcept
    {
        return *this = std::move(Option<T>{none});
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> &&
            std::is_nothrow_invocable_v<NoneF> &&
            std::is_same_v<std::invoke_result_t<SomeF, T&>, void> &&
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match(SomeF&& some_func, NoneF&& none_func) noexcept
    {
        Either<T, None>::match
        (
            std::forward<SomeF>(some_func),
            [none_func = std::forward<NoneF>(none_func)] (None) noexcept
            {
                none_func();
            }
        );
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, const T&> &&
            std::is_nothrow_invocable_v<NoneF> &&
            std::is_same_v<std::invoke_result_t<SomeF, const T&>, void> &&
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match(SomeF&& some_func, NoneF&& none_func) const noexcept
    {
        Either<T, None>::match
        (
            std::forward<SomeF>(some_func),
            [none_func = std::forward<NoneF>(none_func)] (None) noexcept
            {
                none_func();
            }
        );
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> &&
            std::is_nothrow_invocable_v<NoneF> &&
            (not std::is_same_v<std::invoke_result_t<SomeF, T&>, void>) &&
            (not std::is_same_v<std::invoke_result_t<NoneF>, void>)
    constexpr auto Option<T>::match(SomeF&& some_func, NoneF&& none_func) noexcept
        -> std::common_type_t<std::invoke_result_t<SomeF, T&>, std::invoke_result_t<NoneF>>
    {
        return Either<T, None>::match
        (
            std::forward<SomeF>(some_func),
            [none_func = std::forward<NoneF>(none_func)] (None) noexcept
            {
                return none_func();
            }
        );
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, const T&> &&
            std::is_nothrow_invocable_v<NoneF> &&
            (not std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>) &&
            (not std::is_same_v<std::invoke_result_t<NoneF>, void>)
    constexpr auto Option<T>::match(SomeF&& some_func, NoneF&& none_func) const noexcept
        -> std::common_type_t<std::invoke_result_t<SomeF, const T&>, std::invoke_result_t<NoneF>>
    {
        return Either<T, None>::match
        (
            std::forward<SomeF>(some_func),
            [none_func = std::forward<NoneF>(none_func)] (None) noexcept
            {
                return none_func();
            }
        );
    }

    template <typename T>
    template <typename SomeF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> &&
            std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
    constexpr void Option<T>::match_some(SomeF&& some_func) noexcept
    {
        Either<T, None>::match_first(std::forward<SomeF>(some_func));
    }

    template <typename T>
    template <typename SomeF>
        requires
            std::is_nothrow_invocable_v<SomeF, const T&> &&
            std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>
    constexpr void Option<T>::match_some(SomeF&& some_func) const noexcept
    {
        Either<T, None>::match_first(std::forward<SomeF>(some_func));
    }

    template <typename T>
    template <typename NoneF>
        requires
            std::is_nothrow_invocable_v<NoneF> &&
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match_none(NoneF&& none_func) noexcept
    {
        if (is_none())
        {
            none_func();
        }
    }

    template <typename T>
    template <typename NoneF>
        requires
            std::is_nothrow_invocable_v<NoneF> &&
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match_none(NoneF&& none_func) const noexcept
    {
        if (is_none())
        {
            none_func();
        }
    }

    template <typename T>
    constexpr bool Option<T>::is_some() const noexcept
    {
        return Either<T, None>::is_first();
    }

    template <typename T>
    constexpr bool Option<T>::is_none() const noexcept
    {
        return Either<T, None>::is_second();
    }

    template <typename T>
    Option<T> some(T v) noexcept
    {
        return Option<T>{std::move(v)};
    }
}
