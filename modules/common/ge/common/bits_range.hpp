#pragma once

#include <concepts>
#include <iterator>

#include <cstdint>

namespace ge
{
    template <typename T>
    struct BitsIteratorSentinel final
    {
    };

    template <typename T>
        requires std::integral<T>
    class BitsIterator final
    {
    public:
        using difference_type = int;
        using value_type = T;

        BitsIterator() noexcept;
        explicit BitsIterator(const T&) noexcept;
        explicit BitsIterator(const T&, uint8_t bit_number) noexcept;

        T operator*() const noexcept;

        BitsIterator& operator++() noexcept;
        BitsIterator operator++(int) noexcept;

        BitsIterator& operator--() noexcept;
        BitsIterator operator--(int) noexcept;

        bool operator==(const BitsIteratorSentinel<T>&) const noexcept;

    private:
        const T* value_;
        uint8_t bit_number_;
    };

    template<class T>
    BitsIterator(const T&) -> BitsIterator<T>;

    template<class T>
    BitsIterator(const T&, uint8_t) -> BitsIterator<T>;

    template <typename T>
    bool operator!=(const BitsIterator<T>&, const BitsIteratorSentinel<T>&) noexcept;

    template <typename T>
    auto to_bits_range(const T&) noexcept;

    template <typename E, typename T>
        requires std::is_enum_v<E> and std::is_same_v<std::underlying_type_t<E>, T>
    auto to_enum_bits_range(const T&) noexcept;

    static_assert(std::input_or_output_iterator<BitsIterator<int>>);
    static_assert(std::sentinel_for<BitsIteratorSentinel<int>, BitsIterator<int>>);
}

#include "bits_range_impl.hpp"
