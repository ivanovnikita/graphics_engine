#pragma once

#include "errors.hpp"
#include "overloaded.hpp"

namespace ge
{
    template <typename... Args>
    template <typename T>
        requires
            (... or std::is_nothrow_constructible_v<Args, T>)
    Errors<Args...>::Errors(T v) noexcept
        : storage{std::move(v)}
    {
    }

    template <typename... Args>
    template <typename... SubArgs>
        requires
            (... and std::is_nothrow_constructible_v<std::variant<Args...>, SubArgs>)
    Errors<Args...>::Errors(Errors<SubArgs...>&& other) noexcept
        : storage
        {
            [&other] () noexcept
            {
                return std::move(other).match
                (
                    [] (auto&& v) noexcept -> std::variant<Args...>
                    {
                        return std::move(v);
                    }
                );
            } ()
        }
    {
    }

    template <typename... Args>
    Errors<Args...>::Errors(Errors&& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>)
        : storage{std::move(other.storage)}
    {
    }

    template <typename... Args>
    Errors<Args...>& Errors<Args...>::operator=(Errors&& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>) and
            (... and std::is_nothrow_move_assignable_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        if (this != &other)
        {
            Errors(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename... Args>
    Errors<Args...>::Errors(const Errors& other) noexcept
        requires
            (... and std::is_nothrow_copy_constructible_v<Args>)
        : storage{other.storage}
    {
    }

    template <typename... Args>
    Errors<Args...>& Errors<Args...>::operator=(const Errors& other) noexcept
        requires
            (... and std::is_nothrow_copy_constructible_v<Args>) and
            (... and std::is_nothrow_copy_assignable_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        if (this != &other)
        {
            Errors(other).swap(*this);
        }

        return *this;
    }

    template <typename... Args>
    void Errors<Args...>::swap(Errors& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        std::swap(storage, other.storage);
    }

    template <typename... Args>
    template <typename... Fs>
    decltype(auto) Errors<Args...>::match(Fs&&... fs) noexcept
    {
        return std::visit
        (
            overloaded
            {
                std::forward<Fs>(fs)...
            },
            storage
        );
    }
}
