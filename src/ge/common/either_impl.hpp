#pragma once

#include "either.hpp"

#include <utility>

namespace ge
{
    template <typename T, typename U>
    Either<T, U>::Either(T v) noexcept
        : tag{either_tag::first}
        , first{std::move(v)}
    {
    }

    template <typename T, typename U>
    Either<T, U>::Either(U v) noexcept
        : tag{either_tag::second}
        , second{std::move(v)}
    {
    }

    template <typename T, typename U>
    Either<T, U>::~Either() noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            first.~T();
            break;
        }
        case either_tag::second:
        {
            second.~U();
            break;
        }
        }
    }

    template <typename T, typename U>
    Either<T, U>::Either(Either&& other) noexcept
        : tag{other.tag}
    {
        switch (tag)
        {
        case either_tag::first:
        {
            new (&first) T(std::move(other.first));
            break;
        }
        case either_tag::second:
        {
            new (&second) U(std::move(other.second));
            break;
        }
        }
    }

    template <typename T, typename U>
    Either<T, U>& Either<T, U>::operator=(Either&& other) noexcept
    {
        if (this != &other)
        {
            Either(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename T, typename U>
    void Either<T, U>::swap(Either& other) noexcept
    {
        if (tag == other.tag)
        {
            switch (tag)
            {
            case either_tag::first:
            {
                std::swap(first, other.first);
                break;
            }
            case either_tag::second:
            {
                std::swap(second, other.second);
                break;
            }
            }
        }
        else
        {
            switch (tag)
            {
            case either_tag::first:
            {
                T tmp{std::move(first)};
                first.~T();
                new (&second) U(std::move(other.second));
                other.second.~U();
                new (&other.first) T(std::move(tmp));
                break;
            }
            case either_tag::second:
            {
                U tmp{std::move(second)};
                second.~U();
                new (&first) T(std::move(other.first));
                other.first.~T();
                new (&other.second) U(std::move(tmp));
                break;
            }
            }

            std::swap(tag, other.tag);
        }
    }

    template <typename T, typename U>
    template <typename FirstF, typename SecondF>
        requires
            std::is_nothrow_invocable_v<FirstF, T&> &&
            std::is_nothrow_invocable_v<SecondF, U&>
    void Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            first_func(first);
            break;
        }
        case either_tag::second:
        {
            second_func(second);
            break;
        }
        }
    }

    template <typename T, typename U>
    template <typename FirstF, typename SecondF>
        requires
            std::is_nothrow_invocable_v<FirstF, const T&> &&
            std::is_nothrow_invocable_v<SecondF, const U&>
    void Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) const noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            first_func(first);
            break;
        }
        case either_tag::second:
        {
            second_func(second);
            break;
        }
        }
    }
}
