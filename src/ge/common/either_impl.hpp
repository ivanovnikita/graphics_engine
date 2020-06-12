#pragma once

#include "either.hpp"

#include <utility>
#include <memory>

namespace ge
{
    template <typename T, typename U>
    constexpr Either<T, U>::Storage::Storage(first_tag, T v) noexcept
        : first{std::move(v)}
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>::Storage::Storage(second_tag, U v) noexcept
        : second{std::move(v)}
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>::Storage::~Storage() noexcept
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>::Either(T v) noexcept
        : tag{either_tag::first}
        , storage{first_tag{}, std::move(v)}
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>::Either(U v) noexcept
        : tag{either_tag::second}
        , storage{second_tag{}, std::move(v)}
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>::~Either() noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            std::destroy_at(&storage.first);
            break;
        }
        case either_tag::second:
        {
            std::destroy_at(&storage.second);
            break;
        }
        }
    }

    template <typename T, typename U>
    constexpr Either<T, U>::Either(Either&& other) noexcept
        : tag{other.tag}
        , storage
        {
            [this, &other] () mutable
            {
                switch (tag)
                {
                case either_tag::first:
                {
                    return Storage{first_tag{}, std::move(other.storage.first)};
                }
                case either_tag::second:
                {
                    return Storage{second_tag{}, std::move(other.storage.second)};
                }
                }
            }()
        }
    {
    }

    template <typename T, typename U>
    constexpr Either<T, U>& Either<T, U>::operator=(Either&& other) noexcept
    {
        if (this != &other)
        {
            Either(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename T, typename U>
    constexpr void Either<T, U>::swap(Either& other) noexcept
    {
        if (tag == other.tag)
        {
            switch (tag)
            {
            case either_tag::first:
            {
                std::swap(storage.first, other.storage.first);
                break;
            }
            case either_tag::second:
            {
                std::swap(storage.second, other.storage.second);
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
                T tmp{std::move(storage.first)};
                std::destroy_at(&storage.first);

                // this line doesn't compile in constexpr context, I don't know why
                std::construct_at(&storage.second, std::move(other.storage.second));

                std::destroy_at(&other.storage.second);
                std::construct_at(&other.storage.first, std::move(tmp));
                break;
            }
            case either_tag::second:
            {
                U tmp{std::move(storage.second)};
                std::destroy_at(&storage.second);
                std::construct_at(&storage.first, std::move(other.storage.first));
                std::destroy_at(&other.storage.first);
                std::construct_at(&other.storage.second, std::move(tmp));
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
            std::is_nothrow_invocable_v<SecondF, U&> &&
            std::is_same_v<std::invoke_result_t<FirstF, T&>, void> &&
            std::is_same_v<std::invoke_result_t<SecondF, U&>, void>
    constexpr void Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            first_func(storage.first);
            break;
        }
        case either_tag::second:
        {
            second_func(storage.second);
            break;
        }
        }
    }

    template <typename T, typename U>
    template <typename FirstF, typename SecondF>
        requires
            std::is_nothrow_invocable_v<FirstF, const T&> &&
            std::is_nothrow_invocable_v<SecondF, const U&> &&
            std::is_same_v<std::invoke_result_t<FirstF, const T&>, void> &&
            std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>
    constexpr void Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) const noexcept
    {
        switch (tag)
        {
        case either_tag::first:
        {
            first_func(storage.first);
            break;
        }
        case either_tag::second:
        {
            second_func(storage.second);
            break;
        }
        }
    }

    template <typename T, typename U>
    template <typename FirstF, typename SecondF>
        requires
            std::is_nothrow_invocable_v<FirstF, T&> &&
            std::is_nothrow_invocable_v<SecondF, U&> &&
            (not std::is_same_v<std::invoke_result_t<FirstF, T&>, void>) &&
            (not std::is_same_v<std::invoke_result_t<SecondF, U&>, void>)
    constexpr auto Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) noexcept
        -> std::common_type_t<std::invoke_result_t<FirstF, T&>, std::invoke_result_t<SecondF, U&>>
    {
        switch (tag)
        {
        case either_tag::first:
        {
            return first_func(storage.first);
        }
        case either_tag::second:
        {
            return second_func(storage.second);
        }
        }
    }

    template <typename T, typename U>
    template <typename FirstF, typename SecondF>
        requires
            std::is_nothrow_invocable_v<FirstF, T&> &&
            std::is_nothrow_invocable_v<SecondF, U&> &&
            (not std::is_same_v<std::invoke_result_t<FirstF, const T&>, void>) &&
            (not std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>)
    constexpr auto Either<T, U>::match(FirstF&& first_func, SecondF&& second_func) const noexcept
        -> std::common_type_t<std::invoke_result_t<FirstF, const T&>, std::invoke_result_t<SecondF, const U&>>
    {
        switch (tag)
        {
        case either_tag::first:
        {
            return first_func(storage.first);
        }
        case either_tag::second:
        {
            return second_func(storage.second);
        }
        }
    }

    template <typename T, typename U>
    template <typename FirstF>
        requires
            std::is_nothrow_invocable_v<FirstF, T&> &&
            std::is_same_v<std::invoke_result_t<FirstF, T&>, void>
    constexpr void Either<T, U>::match_first(FirstF&& first_func) noexcept
    {
        if (tag == either_tag::first)
        {
            first_func(storage.first);
        }
    }

    template <typename T, typename U>
    template <typename FirstF>
        requires
            std::is_nothrow_invocable_v<FirstF, const T&> &&
            std::is_same_v<std::invoke_result_t<FirstF, const T&>, void>
    constexpr void Either<T, U>::match_first(FirstF&& first_func) const noexcept
    {
        if (tag == either_tag::first)
        {
            first_func(storage.first);
        }
    }

    template <typename T, typename U>
    template <typename SecondF>
        requires
            std::is_nothrow_invocable_v<SecondF, U&> &&
            std::is_same_v<std::invoke_result_t<SecondF, U&>, void>
    constexpr void Either<T, U>::match_second(SecondF&& second_func) noexcept
    {
        if (tag == either_tag::second)
        {
            second_func(storage.second);
        }
    }

    template <typename T, typename U>
    template <typename SecondF>
        requires
            std::is_nothrow_invocable_v<SecondF, const U&> &&
            std::is_same_v<std::invoke_result_t<SecondF, const U&>, void>
    constexpr void Either<T, U>::match_second(SecondF&& second_func) const noexcept
    {
        if (tag == either_tag::second)
        {
            second_func(storage.second);
        }
    }

    template <typename T, typename U>
    constexpr bool Either<T, U>::is_first() const noexcept
    {
        return tag == either_tag::first;
    }

    template <typename T, typename U>
    constexpr bool Either<T, U>::is_second() const noexcept
    {
        return tag == either_tag::second;
    }
}
