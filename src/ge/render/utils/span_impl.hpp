#pragma once

#include "ge/render/utils/span.hpp"

#include <iterator>
#include <type_traits>

namespace ge
{
    template <typename T>
    constexpr Span<T>::Span() noexcept
        : data_(nullptr)
        , size_(0)
    { }

    template <typename T>
    template <typename Container>
    constexpr Span<T>::Span(Container& container)
        : data_(std::data(container))
        , size_(std::size(container))
    {
        using ContainerValue = std::remove_pointer_t<decltype(std::data(container))>;
        static_assert
        (
              std::is_convertible_v<ContainerValue (*)[], T (*)[]>
            , "Given container can't be viewed as an array using pointer of T* type."
        );
    }

    template <typename T>
    constexpr auto Span<T>::operator[](index_type i) const -> reference
    {
        return data_[i];
    }

    template <typename T>
    constexpr auto Span<T>::begin() const noexcept -> iterator
    {
        return data_;
    }

    template <typename T>
    constexpr auto Span<T>::end() const noexcept -> iterator
    {
        return data_ + size_;
    }

    template <typename T>
    constexpr auto Span<T>::data() const noexcept -> pointer
    {
        return data_;
    }

    template <typename T>
    constexpr auto Span<T>::size() const noexcept -> index_type
    {
        return size_;
    }

    template <typename T>
    constexpr auto Span<T>::empty() const noexcept -> bool
    {
        return size_ == 0;
    }
}
