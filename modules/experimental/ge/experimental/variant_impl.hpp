#pragma once

#include "variant.hpp"
#include "ge/common/overloaded.hpp"

namespace ge
{
    template <typename... Args>
    template <typename T>
        requires
            (... or std::is_nothrow_constructible_v<Args, T>)
    Variant<Args...>::Variant(T v) noexcept
        : storage{std::move(v)}
    {
    }

    template <typename... Args>
    Variant<Args...>::Variant(Variant&& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>)
        : storage{std::move(other.storage)}
    {
    }

    template <typename... Args>
    Variant<Args...>& Variant<Args...>::operator=(Variant&& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>) and
            (... and std::is_nothrow_move_assignable_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        if (this != &other)
        {
            Variant(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename... Args>
    Variant<Args...>::Variant(const Variant& other) noexcept
        requires
            (... and std::is_nothrow_copy_constructible_v<Args>)
        : storage{other.storage}
    {
    }

    template <typename... Args>
    Variant<Args...>& Variant<Args...>::operator=(const Variant& other) noexcept
        requires
            (... and std::is_nothrow_copy_constructible_v<Args>) and
            (... and std::is_nothrow_copy_assignable_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        if (this != &other)
        {
            Variant(other).swap(*this);
        }

        return *this;
    }

    template <typename... Args>
    void Variant<Args...>::swap(Variant& other) noexcept
        requires
            (... and std::is_nothrow_move_constructible_v<Args>) and
            (... and std::is_nothrow_swappable_v<Args>)
    {
        std::swap(storage, other.storage);
    }

    template <typename... Args>
    [[ nodiscard ]] auto Variant<Args...>::copy_from(const Variant& other) noexcept
        -> Result
        <
            Variant,
            CopyFromError<Args...>
        >
        requires
            (... and std::is_nothrow_move_constructible_v<Args>) and
            (... and (CopyableFrom<Args> or std::is_nothrow_copy_constructible_v<Args>))
    {
        // TODO
    }

    template <typename... Args>
    template <typename... Fs>
    decltype(auto) Variant<Args...>::match(Fs&&... fs) noexcept
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
