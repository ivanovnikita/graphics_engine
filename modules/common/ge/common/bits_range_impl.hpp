#pragma once

#include "bits_range.hpp"

#include <ranges>

#include <cassert>

namespace ge
{
    template <typename T>
        requires std::integral<T>
    BitsIterator<T>::BitsIterator() noexcept
        : value_{nullptr}
        , bit_number_{8 * sizeof(T)}
    {
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T>::BitsIterator(const T& value) noexcept
        : value_{&value}
        , bit_number_{0}
    {
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T>::BitsIterator(const T& value, const uint8_t bit_number) noexcept
        : value_{&value}
        , bit_number_{bit_number}
    {
        assert(bit_number_ < 8 * sizeof(T));
    }

    template <typename T>
        requires std::integral<T>
    T BitsIterator<T>::operator*() const noexcept
    {
        assert(bit_number_ < 8 * sizeof(T));
        assert(value_ != nullptr);

        const T bit_mask = 1 << bit_number_;
        return *value_ & bit_mask;
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T>& BitsIterator<T>::operator++() noexcept
    {
        assert(bit_number_ < 8 * sizeof(T));
        ++bit_number_;
        return *this;
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T> BitsIterator<T>::operator++(int) noexcept
    {
        BitsIterator<T> tmp{*this};
        ++(*this);
        return tmp;
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T>& BitsIterator<T>::operator--() noexcept
    {
        assert(bit_number_ > 0);
        --bit_number_;
        return *this;
    }

    template <typename T>
        requires std::integral<T>
    BitsIterator<T> BitsIterator<T>::operator--(int) noexcept
    {
        BitsIterator<T> tmp{*this};
        --(*this);
        return tmp;
    }

    template <typename T>
        requires std::integral<T>
    bool BitsIterator<T>::operator==(const BitsIteratorSentinel<T>&) const noexcept
    {
        return bit_number_ == 8 * sizeof(T);
    }

    template <typename T>
    bool operator!=(const BitsIterator<T>& it, const BitsIteratorSentinel<T>& sentinel) noexcept
    {
        return not (it == sentinel);
    }

    template <typename T>
    auto to_bits_range(const T& value) noexcept
    {
        return std::ranges::subrange{BitsIterator<T>{value}, BitsIteratorSentinel<T>{}};
    }
}
