#pragma once

#include "concepts.hpp"
#include "result.hpp"

#include <cstddef>

namespace ge
{
    template <typename T, size_t N>
    class View final
    {
    public:
        enum class IntervalsError
        {
            OriginEndBeforeOriginBegin,
            ViewEndBeforeViewBegin,
            ViewBeginBeforeOriginBegin,
            ViewEndAfterOriginEnd
        };

        struct SizeError final
        {
            size_t size_from_type;
            size_t size_from_pointers;
        };

        [[ nodiscard ]] static Result
        <
            View,
            Errors
            <
                IntervalsError,
                SizeError
            >
        > create
        (
            T* origin_begin,
            T* origin_end,
            T* view_begin,
            T* view_end
        ) noexcept;

        View(View&&) noexcept = default;
        View& operator=(View&&) noexcept = default;

        View(const View&) noexcept = default;
        View& operator=(const View&) noexcept = default;

        template <size_t Index>
            requires (Index < N)
        T& at() noexcept;

        template <size_t Index>
            requires (Index < N)
        const T& at() const noexcept;

        struct ShiftRightError final
        {
            size_t requested_shift;
            size_t max_possible_shift;
        };

        [[ nodiscard ]] Result
        <
            View,
            Errors<ShiftRightError>
        > shift_right(const size_t offset) const noexcept;

    private:
        View
        (
            T* origin_begin,
            T* origin_end,
            T* view_begin,
            T* view_end
        ) noexcept;

        T* origin_begin;
        T* origin_end;
        T* view_begin;
        T* view_end;
    };
}

#include "view_impl.hpp"
