#pragma once

#include "allocator.h"
#include "concepts.hpp"
#include "create_error.hpp"
#include "option.hpp"
#include "result.hpp"
#include "view.hpp"

#include <span>

namespace ge
{
    template <typename T>
    class Vector final
    {
    public:
        using ValueType = T;

        Vector() noexcept;

        ~Vector() noexcept
            requires
                std::is_nothrow_destructible_v<T>;

        Vector(Vector&&) noexcept;
        Vector& operator=(Vector&&) noexcept;

        Vector(const Vector&) = delete;
        Vector& operator=(const Vector&) = delete;

        void swap(Vector&) noexcept;

        [[ nodiscard ]] static Result
        <
            Vector<T>,
            Errors<Allocator::AllocationError>
        > create
        (
            Allocator&,
            size_t capacity
        ) noexcept;

        [[ nodiscard ]] static Result
        <
            Vector<T>,
            Errors
            <
                Allocator::AllocationError,
                CopyFromError<T>
            >
        > copy_from
        (
            Allocator&,
            const Vector&
        ) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>);

        struct IndexError final
        {
            size_t requested_index;
            size_t current_size;

            static constexpr std::string_view error_message{"requested index exceeds vector size"};
        };

        [[ nodiscard ]] Result
        <
            Ref<T>,
            Errors<IndexError>
        > operator[](size_t ind) noexcept;

        [[ nodiscard ]] Result
        <
            Ref<const T>,
            Errors<IndexError>
        > operator[](size_t ind) const noexcept;

        enum class IndexIntervalError
        {
            IndexFromExceedsMax,
            IndexToExceedsMax
        };

        struct ViewError final
        {
            IndexIntervalError interval_error;
            size_t requested_index;
            size_t current_size;

            static constexpr std::string_view error_message{"requested indcies exceed vector size"};
        };

        template <size_t N>
        [[ nodiscard ]] Result
        <
            View<T, N>,
            Errors<ViewError>
        > view(size_t ind_from) noexcept;

        template <size_t N>
        [[ nodiscard ]] Result
        <
            View<const T, N>,
            Errors<ViewError>
        > view(size_t ind_from) const noexcept;

        [[ nodiscard ]] Option
        <
            Errors<Allocator::AllocationError>
        > reserve
        (
            Allocator&,
            size_t count
        ) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        [[ nodiscard ]] Option
        <
            Errors<Allocator::AllocationError>
        > push_back
        (
            Allocator&,
            T
        ) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        template <typename... Args>
        [[ nodiscard ]] Option
        <
            Errors<Allocator::AllocationError>
        > construct_at_end
        (
            Allocator&,
            Args&&...
        ) noexcept
            requires
                std::is_nothrow_constructible_v<T, Args...>;

        struct PushToReservedError final
        {
            size_t reserved_space;
            size_t required_space;

            static constexpr std::string_view error_message{"reserved space is not so big as required"};
        };

        [[ nodiscard ]] Option
        <
            Errors<PushToReservedError>
        > push_back_to_reserved(T) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        template <typename... Args>
        [[ nodiscard ]] Option
        <
            Errors<PushToReservedError>
        > construct_at_reserved_end(Args&&...) noexcept
            requires
                std::is_nothrow_constructible_v<T, Args...>;

        void destroy_all_elements() noexcept;
        void free_all_memory() noexcept;

        size_t size() const noexcept;
        size_t capacity() const noexcept;

        template <typename F>
            requires
                std::is_nothrow_invocable_v<F, T&> and
                std::is_same_v<std::invoke_result_t<F, T&>, void>
        constexpr void for_each(F&& func) noexcept;

        template <typename F>
            requires
                std::is_nothrow_invocable_v<F, const T&> and
                std::is_same_v<std::invoke_result_t<F, const T&>, void>
        constexpr void for_each(F&& func) const noexcept;

        template <typename F>
            requires
                std::is_nothrow_invocable_v<F, T&> and
                std::is_same_v<std::invoke_result_t<F, T&>, bool>
        constexpr void for_each_until(F&& func) noexcept;

        template <typename F>
            requires
                std::is_nothrow_invocable_v<F, const T&> and
                std::is_same_v<std::invoke_result_t<F, const T&>, bool>
        constexpr void for_each_until(F&& func) noexcept;

    private:
        Vector
        (
            Allocator::Ptr storage,
            size_t capacity
        ) noexcept;

        [[ nodiscard ]] Option
        <
            Errors<Allocator::AllocationError>
        > reserve_for_new_one(Allocator&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        Option<Allocator::Ptr> storage_;
        size_t capacity_;
        size_t size_;
    };

}

#include "vector_impl.hpp"
