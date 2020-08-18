#pragma once

#include "copy_from_error.hpp"
#include "errors.hpp"

namespace ge
{
    struct Ok final {};

    template <typename OkT, typename PossibleErrors>
    class Result final
    {
    public:
        using T = OkT;
        using Err = PossibleErrors;

        Result(T v) noexcept
            requires
                std::is_nothrow_move_constructible_v<T>;

        Result(Err v) noexcept
            requires
                std::is_nothrow_move_constructible_v<Err>;

        template <typename U>
        Result(U u) noexcept
            requires
                std::is_nothrow_convertible_v<U, Err>;

        ~Result() noexcept
            requires
                std::is_nothrow_destructible_v<T> and
                std::is_nothrow_destructible_v<Err>;

        Result(Result&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<Err>;

        Result& operator=(Result&&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<Err> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<Err>;

        Result(const Result&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T> and
                std::is_nothrow_copy_constructible_v<Err>;

        Result& operator=(const Result&) noexcept
            requires
                std::is_nothrow_copy_constructible_v<T> and
                std::is_nothrow_copy_constructible_v<Err> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<Err>;

        void swap(Result&) noexcept
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<Err> and
                std::is_nothrow_swappable_v<T> and
                std::is_nothrow_swappable_v<Err>;

        [[ nodiscard ]] static constexpr auto copy_from(const Result&) noexcept
            -> Result
            <
                Result,
                CopyFromError<T, Err>
            >
            requires
                std::is_nothrow_move_constructible_v<T> and
                std::is_nothrow_move_constructible_v<Err> and
                (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>) and
                (CopyableFrom<Err> or std::is_nothrow_copy_constructible_v<Err>);

        template <typename OkF, typename ErrF>
            requires
                std::is_nothrow_invocable_v<OkF, T&> and
                std::is_nothrow_invocable_v<ErrF, Err&>
        constexpr auto match(OkF&& ok_func, ErrF&& err_func) & noexcept
            -> std::common_type_t<std::invoke_result_t<OkF, T&>, std::invoke_result_t<ErrF, Err&>>;

        template <typename OkF, typename ErrF>
            requires
                std::is_nothrow_invocable_v<OkF, T&&> and
                std::is_nothrow_invocable_v<ErrF, Err&&>
        constexpr auto match(OkF&& ok_func, ErrF&& err_func) && noexcept
            -> std::common_type_t<std::invoke_result_t<OkF, T&&>, std::invoke_result_t<ErrF, Err&&>>;

        template <typename OkF, typename ErrF>
            requires
                std::is_nothrow_invocable_v<OkF, const T&> and
                std::is_nothrow_invocable_v<ErrF, const Err&>
        constexpr auto match(OkF&& ok_func, ErrF&& err_func) const noexcept
            -> std::common_type_t<std::invoke_result_t<OkF, const T&>, std::invoke_result_t<ErrF, const Err&>>;


        template <typename OkF>
            requires
                std::is_nothrow_invocable_v<OkF, T&> and
                std::is_same_v<std::invoke_result_t<OkF, T&>, void>
        constexpr void match_ok(OkF&& ok_func) & noexcept;

        template <typename OkF>
            requires
                std::is_nothrow_invocable_v<OkF, T&&> and
                std::is_same_v<std::invoke_result_t<OkF, T&&>, void>
        constexpr void match_ok(OkF&& ok_func) && noexcept;

        template <typename OkF>
            requires
                std::is_nothrow_invocable_v<OkF, const T&> and
                std::is_same_v<std::invoke_result_t<OkF, const T&>, void>
        constexpr void match_ok(OkF&& ok_func) const noexcept;

        template <typename ErrF>
            requires
                std::is_nothrow_invocable_v<ErrF, Err&> and
                std::is_same_v<std::invoke_result_t<ErrF, Err&>, void>
        constexpr void match_error(ErrF&& err_func) & noexcept;

        template <typename ErrF>
            requires
                std::is_nothrow_invocable_v<ErrF, Err&&> and
                std::is_same_v<std::invoke_result_t<ErrF, Err&&>, void>
        constexpr void match_error(ErrF&& err_func) && noexcept;

        template <typename ErrF>
            requires
                std::is_nothrow_invocable_v<ErrF, const Err&> and
                std::is_same_v<std::invoke_result_t<ErrF, const Err&>, void>
        constexpr void match_error(ErrF&& err_func) const noexcept;

        constexpr bool is_ok() const noexcept;
        constexpr bool is_error() const noexcept;

        template <typename ThenF>
            requires
                std::is_nothrow_invocable_v<ThenF, T&&> and
                (not Resultable<std::invoke_result_t<ThenF, T&&>>) and
                (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>)
        constexpr auto then(ThenF&& then_func) && noexcept
            -> Result<std::invoke_result_t<ThenF, T&&>, Err>;

        template <typename ThenF>
            requires
                std::is_nothrow_invocable_v<ThenF, T&&> and
                std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>
        constexpr auto then(ThenF&& then_func) && noexcept
            -> Result<Ok, Err>;

        template <typename ThenF>
            requires
                std::is_nothrow_invocable_v<ThenF, T&&> and
                Resultable<std::invoke_result_t<ThenF, T&&>>
        constexpr auto then(ThenF&& then_func) && noexcept
            -> Result
            <
                typename std::invoke_result_t<ThenF, T&&>::T,
                MergeUnique
                <
                    Err,
                    typename std::invoke_result_t<ThenF, T&&>::Err
                >
            >;

        template <typename OnErrorF>
            requires
                std::is_nothrow_invocable_v<OnErrorF, Err&&> and
                (not std::is_same_v<std::invoke_result_t<OnErrorF, Err&&>, void>)
        constexpr auto on_error(OnErrorF&& on_error_func) && noexcept
            -> Result<T, std::invoke_result_t<OnErrorF, Err&&>>;

    private:
        enum class result_tag
        {
            ok,
            err
        };

        struct ok_tag final {};
        struct err_tag final {};

        result_tag tag;

        union Storage
        {
            constexpr Storage(ok_tag, T v) noexcept;
            constexpr Storage(err_tag, Err v) noexcept;
            constexpr ~Storage() noexcept;

            T ok;
            Err err;
        } storage;
    };
}

#include "result_impl.hpp"
