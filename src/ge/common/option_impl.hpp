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
    Option<T> some(T&& v) noexcept
    {
        return Option<T>{std::forward<T>(v)};
    }
}
