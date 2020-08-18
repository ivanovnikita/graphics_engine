#pragma once

#include "option.hpp"

namespace ge
{
    template <typename T>
    constexpr Option<T>::Storage::Storage(some_tag, T v) noexcept
        : some{std::move(v)}
    {
    }

    template <typename T>
    constexpr Option<T>::Storage::Storage(none_tag) noexcept
        : none{}
    {
    }

    template <typename T>
    constexpr Option<T>::Storage::~Storage() noexcept
    {
    }

    template <typename T>
    constexpr Option<T>::Option(T v) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
        : contains_value{true}
        , storage{some_tag{}, std::move(v)}
    {
    }

    template <typename T>
    constexpr Option<T>::Option(None) noexcept
        : contains_value{false}
        , storage{none_tag{}}
    {
    }

    template <typename T>
    constexpr Option<T>::Option(Result<Ok, T> e) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
        : contains_value{e.is_error()}
        , storage
        {
            std::move(e)
                .match
                (
                    [] (Ok) noexcept
                    {
                        return Storage{none_tag{}};
                    },
                    [] (T&& error) noexcept
                    {
                        return Storage{some_tag{}, std::move(error)};
                    }
                )
        }
    {
    }

    template <typename T>
    constexpr Option<T>::~Option() noexcept
        requires
            std::is_nothrow_destructible_v<T>
    {
        if (contains_value)
        {
            std::destroy_at(&storage.some);
        }
    }

    template <typename T>
    constexpr Option<T>::Option(Option&& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
        : contains_value{other.contains_value}
        , storage
        {
            [this, &other] () mutable
            {
                if (other.contains_value)
                {
                    return Storage{some_tag{}, std::move(other.storage.some)};
                }

                return Storage{none_tag{}};
            } ()
        }
    {
        if (other.contains_value)
        {
            other = Option{none};
        }
    }

    template <typename T>
    constexpr Option<T>& Option<T>::operator=(Option&& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            std::is_nothrow_swappable_v<T>
    {
        if (this != &other)
        {
            Option{std::move(other)}.swap(*this);
        }

        return *this;
    }

    template <typename T>
    constexpr Option<T>::Option(const Option& other) noexcept
        requires
            std::is_nothrow_copy_constructible_v<T>
        : contains_value{other.contains_value}
        , storage
        {
            [this, &other]
            {
                if (other.contains_value)
                {
                    return Storage{some_tag{}, other.storage.some};
                }

                return Storage{none_tag{}};
            }
        }
    {
    }

    template <typename T>
    constexpr Option<T>& Option<T>::operator=(const Option& other) noexcept
        requires
            std::is_nothrow_copy_constructible_v<T> and
            std::is_nothrow_swappable_v<T>
    {
        if (this != &other)
        {
            Option{other}.swap(*this);
        }

        return *this;
    }

    template <typename T>
    constexpr Option<T>& Option<T>::operator=(None) noexcept
    {
        if (contains_value)
        {
            std::destroy_at(&storage.some);
            std::construct_at(&storage.none, none);
            contains_value = false;
        }

        return *this;
    }

    template <typename T>
    constexpr void Option<T>::swap(Option& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            std::is_nothrow_swappable_v<T>
    {
        if (contains_value == other.contains_value)
        {
            if (contains_value)
            {
                std::swap(storage.some, other.storage.some);
            }
        }
        else
        {
            if (contains_value)
            {
                std::construct_at(&other.storage.some, std::move(storage.some));
                std::destroy_at(&storage.some);
                std::construct_at(&storage.none, none);
            }
            else
            {
                std::construct_at(&storage.some, std::move(other.storage.some));
                std::destroy_at(&other.storage.some);
                std::construct_at(&other.storage.none, none);
            }

            std::swap(contains_value, other.contains_value);
        }
    }

    template <typename T>
    [[ nodiscard ]] constexpr auto Option<T>::copy_from(const Option& other) noexcept
        -> Result
        <
            Option,
            CopyFromError<T>
        >
        requires
            std::is_nothrow_move_constructible_v<T> and
            (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>)
    {
        using ErrorT = typename CopyFromError<Filter<CopyableFromT, std::tuple<T>>>::Error;

        if (other.contains_value)
        {
            if constexpr (CopyableFrom<T>)
            {
                return T::copy_from(other.storage.some)
                    .match
                    (
                        [] (T&& value) noexcept -> Result<Option, ErrorT>
                        {
                            return Option{std::move(value)};
                        },
                        [] (auto&& error) noexcept -> Result<Option, ErrorT>
                        {
                            return std::move(error);
                        }
                    );
            }
            else
            {
                return Option{other.storage.some};
            }
        }

        return Option<T>{none};
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> and
            std::is_nothrow_invocable_v<NoneF>
    constexpr auto Option<T>::match(SomeF&& some_func, NoneF&& none_func) & noexcept
        -> std::common_type_t<std::invoke_result_t<SomeF, T&>, std::invoke_result_t<NoneF>>
    {
        if (contains_value)
        {
            return some_func(storage.some);
        }

        return none_func();
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> and
            std::is_nothrow_invocable_v<NoneF>
    constexpr auto Option<T>::match(SomeF&& some_func, NoneF&& none_func) && noexcept
        -> std::common_type_t<std::invoke_result_t<SomeF, T&&>, std::invoke_result_t<NoneF>>
    {
        if (contains_value)
        {
            decltype(auto) result = some_func(std::move(storage.some));
            *this = none;
            return result;
        }

        return none_func();
    }

    template <typename T>
    template <typename SomeF, typename NoneF>
        requires
            std::is_nothrow_invocable_v<SomeF, const T&> and
            std::is_nothrow_invocable_v<NoneF>
    constexpr auto Option<T>::match(SomeF&& some_func, NoneF&& none_func) const noexcept
        -> std::common_type_t<std::invoke_result_t<SomeF, const T&>, std::invoke_result_t<NoneF>>
    {
        if (contains_value)
        {
            return some_func(storage.some);
        }

        return none_func();
    }

    template <typename T>
    template <typename SomeF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> and
            std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
    constexpr void Option<T>::match_some(SomeF&& some_func) & noexcept
    {
        if (contains_value)
        {
            some_func(storage.some);
        }
    }

    template <typename T>
    template <typename SomeF>
        requires
            std::is_nothrow_invocable_v<SomeF, T&> and
            std::is_same_v<std::invoke_result_t<SomeF, T&>, void>
    constexpr void Option<T>::match_some(SomeF&& some_func) && noexcept
    {
        if (contains_value)
        {
            some_func(storage.some);
            *this = none;
        }
    }

    template <typename T>
    template <typename SomeF>
        requires
            std::is_nothrow_invocable_v<SomeF, const T&> and
            std::is_same_v<std::invoke_result_t<SomeF, const T&>, void>
    constexpr void Option<T>::match_some(SomeF&& some_func) const noexcept
    {
        if (contains_value)
        {
            some_func(storage.some);
        }
    }

    template <typename T>
    template <typename NoneF>
        requires
            std::is_nothrow_invocable_v<NoneF> and
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match_none(NoneF&& none_func) noexcept
    {
        if (not contains_value)
        {
            none_func();
        }
    }

    template <typename T>
    template <typename NoneF>
        requires
            std::is_nothrow_invocable_v<NoneF> and
            std::is_same_v<std::invoke_result_t<NoneF>, void>
    constexpr void Option<T>::match_none(NoneF&& none_func) const noexcept
    {
        if (not contains_value)
        {
            none_func();
        }
    }

    template <typename T>
    constexpr bool Option<T>::is_some() const noexcept
    {
        return contains_value;
    }

    template <typename T>
    constexpr bool Option<T>::is_none() const noexcept
    {
        return not contains_value;
    }

    template <typename T>
    Option<T> some(T v) noexcept
    {
        return Option<T>{std::move(v)};
    }

    template <typename T>
    template <typename ThenF>
        requires
            std::is_nothrow_invocable_v<ThenF, T&&> and
            (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>) and
            (not Optionable<std::invoke_result_t<ThenF, T&&>>)
    constexpr auto Option<T>::then(ThenF&& then_func) && noexcept
        -> Option<std::invoke_result_t<ThenF, T&&>>
    {
        if (contains_value)
        {
            auto result = then_func(storage.some);
            *this = none;
            return std::move(result);
        }

        return none;
    }

    template <typename T>
    template <typename ThenF>
        requires
            std::is_nothrow_invocable_v<ThenF, T&&> and
            (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>) and
            Optionable<std::invoke_result_t<ThenF, T&&>>
    constexpr auto Option<T>::then(ThenF&& then_func) && noexcept
        -> Option<std::invoke_result_t<ThenF, T&&>>
    {
        using R = Option<std::invoke_result_t<ThenF, T&&>>;

        if (contains_value)
        {
            auto result = then_func(storage.some);
            *this = none;
            return std::move(result)
                .match
                (
                    [] (auto&& v) noexcept -> R
                    {
                        return std::move(v);
                    },
                    [] () noexcept -> R
                    {
                        return none;
                    }
                );
        }

        return none;
    }
}
