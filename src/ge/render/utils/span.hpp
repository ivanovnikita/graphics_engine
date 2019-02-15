#pragma once

#include <cstddef>

namespace ge
{
    template <typename T>
    class Span final
    {
    public:
        using index_type = size_t;
        using pointer = T*;
        using iterator = T*;
        using reference = T&;

        constexpr Span() noexcept;
        constexpr Span(const Span<T>& other) noexcept = default;

        template <typename Container>
        constexpr Span(Container&);

        constexpr Span<T>& operator=(const Span<T>& other) noexcept = default;
        constexpr reference operator[](index_type i) const;

        constexpr iterator begin() const noexcept;
        constexpr iterator end() const noexcept;
        constexpr pointer data() const noexcept;
        constexpr index_type size() const noexcept;
        constexpr bool empty() const noexcept;

    private:
        T* data_;
        index_type size_;
    };
}

#include "ge/render/utils/span_impl.hpp"
