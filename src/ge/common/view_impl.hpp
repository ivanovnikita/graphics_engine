#pragma once

#include "view.hpp"

#include <cassert>

namespace ge
{
    template <typename T, size_t N>
    auto View<T, N>::create
    (
        T* const origin_begin,
        T* const origin_end,
        T* const view_begin,
        T* const view_end
    ) noexcept -> Result
    <
        View,
        Errors
        <
            IntervalsError,
            SizeError
        >
    >
    {
        if (origin_end < origin_begin)
        {
            return IntervalsError::OriginEndBeforeOriginBegin;
        }

        if (view_end < view_begin)
        {
            return IntervalsError::ViewEndBeforeViewBegin;
        }

        if (view_begin < origin_begin)
        {
            return IntervalsError::ViewBeginBeforeOriginBegin;
        }

        if (view_end > origin_end)
        {
            return IntervalsError::ViewEndAfterOriginEnd;
        }

        if (size_t size = view_end - view_begin; size != N)
        {
            return SizeError
            {
                .size_from_type = N,
                .size_from_pointers = size
            };
        }

        return View
        {
            origin_begin,
            origin_end,
            view_begin,
            view_end
        };
    }

    template <typename T, size_t N>
    View<T, N>::View
    (
        T* origin_begin,
        T* origin_end,
        T* view_begin,
        T* view_end
    ) noexcept
        : origin_begin{origin_begin}
        , origin_end{origin_end}
        , view_begin{view_begin}
        , view_end{view_end}
    {
    }

    template <typename T, size_t N>
    template <size_t Index>
        requires (Index < N)
    T& View<T, N>::at() noexcept
    {
        return view_begin[Index];
    }

    template <typename T, size_t N>
    template <size_t Index>
        requires (Index < N)
    const T& View<T, N>::at() const noexcept
    {
        return view_begin[Index];
    }

    template <typename T, size_t N>
    auto View<T, N>::shift_right(const size_t offset) const noexcept
        -> Result<View, Errors<ShiftRightError>>
    {
        T* new_view_end = view_end + offset;
        if (new_view_end > origin_end)
        {
            return ShiftRightError
            {
                .requested_shift = offset,
                .max_possible_shift = origin_end - view_end
            };
        }

        return View
        (
            origin_begin,
            origin_end,
            view_begin + offset,
            new_view_end
        );
    }
}
