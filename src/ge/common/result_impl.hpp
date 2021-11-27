#pragma once

#include "result.hpp"

#include <memory>

namespace ge
{
    template <typename T, typename Err>
    constexpr Result<T, Err>::Storage::Storage(ok_tag, T v) noexcept
        : ok{std::move(v)}
    {
    }

    template <typename T, typename Err>
    constexpr Result<T, Err>::Storage::Storage(err_tag, Err v) noexcept
        : err{std::move(v)}
    {
    }

    template <typename T, typename Err>
    constexpr Result<T, Err>::Storage::~Storage() noexcept
    {
    }

    template <typename T, typename Err>
    Result<T, Err>::Result(T v) noexcept
        requires
            std::is_nothrow_move_constructible_v<T>
        : tag{result_tag::ok}
        , storage{ok_tag{}, std::move(v)}
    {
    }

    template <typename T, typename Err>
    Result<T, Err>::Result(Err v) noexcept
        requires
            std::is_nothrow_move_constructible_v<Err>
        : tag{result_tag::err}
        , storage{err_tag{}, std::move(v)}
    {
    }

    template <typename T, typename Err>
    template <typename U>
    Result<T, Err>::Result(U u) noexcept
        requires
            std::is_nothrow_constructible_v<Err, U>
        : tag{result_tag::err}
        , storage{err_tag{}, Err{std::move(u)}}
    {
    }

//    template <typename T, typename Err>
//    template <typename... OtherErr>
//    Result<T, Err>::Result(Result<T, Errors<OtherErr...>>&& other) noexcept
//        requires
//            (... and std::is_nothrow_move_constructible_v<Err, OtherErr>)
//        : tag{other.tag}
//        , storage
//        {
//            [this, &other] () mutable
//            {
//                switch (tag)
//                {
//                case result_tag::ok:
//                {
//                    return Storage{ok_tag{}, std::move(other.storage.ok)};
//                }
//                case result_tag::err:
//                {
//                    return Storage
//                    {
//                        err_tag{},
//                        std::move(other.storage.err).
//                            match
//                            (
//                                [] (auto&& v) noexcept
//                                {
//                                    return Err{std::move(v)};
//                                }
//                            )
//                    };
//                }
//                }
//            } ()
//        }
//    {
//    }

    template <typename T, typename Err>
    Result<T, Err>::~Result() noexcept
        requires
            std::is_nothrow_destructible_v<T> and
            std::is_nothrow_destructible_v<Err>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            std::destroy_at(&storage.ok);
            break;
        }
        case result_tag::err:
        {
            std::destroy_at(&storage.err);
            break;
        }
        }
    }

    template <typename T, typename Err>
    Result<T, Err>::Result(Result&& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            std::is_nothrow_move_constructible_v<Err>
        : tag{other.tag}
        , storage
        {
            [this, &other] () mutable
            {
                switch (tag)
                {
                case result_tag::ok:
                {
                    return Storage{ok_tag{}, std::move(other.storage.ok)};
                }
                case result_tag::err:
                {
                    return Storage{err_tag{}, std::move(other.storage.err)};
                }
                }
            } ()
        }
    {
    }

    template <typename T, typename Err>
    Result<T, Err>& Result<T, Err>::operator=(Result&& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            std::is_nothrow_move_constructible_v<Err> and
            std::is_nothrow_swappable_v<T> and
            std::is_nothrow_swappable_v<Err>
    {
        if (this != &other)
        {
            Result(std::move(other)).swap(*this);
        }

        return *this;
    }

    template <typename T, typename Err>
    Result<T, Err>::Result(const Result& other) noexcept
        requires
            std::is_nothrow_copy_constructible_v<T> and
            std::is_nothrow_copy_constructible_v<Err>
        : tag{other.tag}
        , storage
        {
            [this, &other] () mutable
            {
                switch (tag)
                {
                case result_tag::ok:
                {
                    return Storage{ok_tag{}, other.storage.ok};
                }
                case result_tag::err:
                {
                    return Storage{err_tag{}, other.storage.err};
                }
                }
            } ()
        }
    {
    }

    template <typename T, typename Err>
    Result<T, Err>& Result<T, Err>::operator=(const Result& other) noexcept
        requires
            std::is_nothrow_copy_constructible_v<T> and
            std::is_nothrow_copy_constructible_v<Err> and
            std::is_nothrow_swappable_v<T> and
            std::is_nothrow_swappable_v<Err>
    {
        if (this != &other)
        {
            Result(other).swap(*this);
        }

        return *this;
    }

    template <typename T, typename Err>
    void Result<T, Err>::swap(Result& other) noexcept
        requires
            std::is_nothrow_move_constructible_v<T> and
            std::is_nothrow_move_constructible_v<Err> and
            std::is_nothrow_swappable_v<T> and
            std::is_nothrow_swappable_v<Err>
    {
        if (tag == other.tag)
        {
            switch (tag)
            {
            case result_tag::ok:
            {
                std::swap(storage.ok, other.storage.ok);
                break;
            }
            case result_tag::err:
            {
                std::swap(storage.err, other.storage.err);
                break;
            }
            }
        }
        else
        {
            switch (tag)
            {
            case result_tag::first:
            {
                T tmp{std::move(storage.ok)};
                std::destroy_at(&storage.ok);
                std::construct_at(&storage.err, std::move(other.storage.err));
                std::destroy_at(&other.storage.err);
                std::construct_at(&other.storage.ok, std::move(tmp));
                break;
            }
            case result_tag::second:
            {
                Err tmp{std::move(storage.err)};
                std::destroy_at(&storage.err);
                std::construct_at(&storage.ok, std::move(other.storage.ok));
                std::destroy_at(&other.storage.ok);
                std::construct_at(&other.storage.err, std::move(tmp));
                break;
            }
            }

            std::swap(tag, other.tag);
        }
    }

    template <typename T, typename Err>
    [[ nodiscard ]] constexpr auto Result<T, Err>::copy_from(const Result& other) noexcept
    -> Result
    <
        Result,
        CopyFromError<T, Err>
    >
    requires
        std::is_nothrow_move_constructible_v<T> and
        std::is_nothrow_move_constructible_v<Err> and
        (CopyableFrom<T> or std::is_nothrow_copy_constructible_v<T>) and
        (CopyableFrom<Err> or std::is_nothrow_copy_constructible_v<Err>)
    {
        using ErrorT = typename CopyFromError<Filter<CopyableFromT, std::tuple<T, Err>>>::Error;

        switch (other.tag)
        {
        case result_tag::ok:
        {
            if constexpr (CopyableFrom<T>)
            {
                return T::copy_from(other.storage.ok)
                    .match
                    (
                        [] (T&& value) noexcept -> Result<Result<T, Err>, ErrorT>
                        {
                            return Result<T, Err>{std::move(value)};
                        },
                        [] (auto&& error) noexcept -> Result<Result<T, Err>, ErrorT>
                        {
                            return Result<T, Err>{std::move(error)};
                        }
                    );
            }
            else
            {
                return Result<T, Err>{other.storage.ok};
            }
        }
        case result_tag::err:
        {
            if constexpr (CopyableFrom<Err>)
            {
                return Err::copy_from(other.storage.err)
                    .match
                    (
                        [] (Err&& value) noexcept -> Result<Result<T, Err>, ErrorT>
                        {
                            return Result<T, Err>{std::move(value)};
                        },
                        [] (auto&& error) noexcept -> Result<Result<T, Err>, ErrorT>
                        {
                            return Result<T, Err>{std::move(error)};
                        }
                    );
            }
            else
            {
                return Result<T, Err>{other.storage.err};
            }
        }
        }
    }

    template <typename T, typename Err>
    template <typename OkF, typename ErrF>
        requires
            std::is_nothrow_invocable_v<OkF, T&> and
            std::is_nothrow_invocable_v<ErrF, Err&>
    constexpr auto Result<T, Err>::match(OkF&& ok_func, ErrF&& err_func) & noexcept
        -> std::common_type_t<std::invoke_result_t<OkF, T&>, std::invoke_result_t<ErrF, Err&>>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            return ok_func(storage.ok);
        }
        case result_tag::err:
        {
            return err_func(storage.err);
        }
        }
    }

    template <typename T, typename Err>
    template <typename OkF, typename ErrF>
        requires
            std::is_nothrow_invocable_v<OkF, T&&> and
            std::is_nothrow_invocable_v<ErrF, Err&&>
    constexpr auto Result<T, Err>::match(OkF&& ok_func, ErrF&& err_func) && noexcept
        -> std::common_type_t<std::invoke_result_t<OkF, T&&>, std::invoke_result_t<ErrF, Err&&>>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            return ok_func(std::move(storage.ok));
        }
        case result_tag::err:
        {
            return err_func(std::move(storage.err));
        }
        }
    }

    template <typename T, typename Err>
    template <typename OkF, typename ErrF>
        requires
            std::is_nothrow_invocable_v<OkF, const T&> and
            std::is_nothrow_invocable_v<ErrF, const Err&>
    constexpr auto Result<T, Err>::match(OkF&& ok_func, ErrF&& err_func) const noexcept
        -> std::common_type_t<std::invoke_result_t<OkF, const T&>, std::invoke_result_t<ErrF, const Err&>>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            return ok_func(storage.ok);
        }
        case result_tag::err:
        {
            return err_func(storage.err);
        }
        }
    }


    template <typename T, typename Err>
    template <typename OkF>
        requires
            std::is_nothrow_invocable_v<OkF, T&> and
            std::is_same_v<std::invoke_result_t<OkF, T&>, void>
    constexpr void Result<T, Err>::match_ok(OkF&& ok_func) & noexcept
    {
        if (tag == result_tag::ok)
        {
            ok_func(storage.ok);
        }
    }

    template <typename T, typename Err>
    template <typename OkF>
        requires
            std::is_nothrow_invocable_v<OkF, T&&> and
            std::is_same_v<std::invoke_result_t<OkF, T&&>, void>
    constexpr void Result<T, Err>::match_ok(OkF&& ok_func) && noexcept
    {
        if (tag == result_tag::ok)
        {
            ok_func(std::move(storage.ok));
        }
    }

    template <typename T, typename Err>
    template <typename OkF>
        requires
            std::is_nothrow_invocable_v<OkF, const T&> and
            std::is_same_v<std::invoke_result_t<OkF, const T&>, void>
    constexpr void Result<T, Err>::match_ok(OkF&& ok_func) const noexcept
    {
        if (tag == result_tag::ok)
        {
            ok_func(storage.ok);
        }
    }

    template <typename T, typename Err>
    template <typename ErrF>
        requires
            std::is_nothrow_invocable_v<ErrF, Err&> and
            std::is_same_v<std::invoke_result_t<ErrF, Err&>, void>
    constexpr void Result<T, Err>::match_error(ErrF&& err_func) & noexcept
    {
        if (tag == result_tag::err)
        {
            err_func(storage.err);
        }
    }

    template <typename T, typename Err>
    template <typename ErrF>
        requires
            std::is_nothrow_invocable_v<ErrF, Err&&> and
            std::is_same_v<std::invoke_result_t<ErrF, Err&&>, void>
    constexpr void Result<T, Err>::match_error(ErrF&& err_func) && noexcept
    {
        if (tag == result_tag::err)
        {
            err_func(std::move(storage.err));
        }
    }

    template <typename T, typename Err>
    template <typename ErrF>
        requires
            std::is_nothrow_invocable_v<ErrF, const Err&> and
            std::is_same_v<std::invoke_result_t<ErrF, const Err&>, void>
    constexpr void Result<T, Err>::match_error(ErrF&& err_func) const noexcept
    {
        if (tag == result_tag::err)
        {
            err_func(storage.err);
        }
    }

    template <typename T, typename Err>
    constexpr bool Result<T, Err>::is_ok() const noexcept
    {
        return tag == result_tag::ok;
    }

    template <typename T, typename Err>
    constexpr bool Result<T, Err>::is_error() const noexcept
    {
        return tag == result_tag::ok;
    }

    template <typename T, typename Err>
    template <typename ThenF>
        requires
            std::is_nothrow_invocable_v<ThenF, T&&> and
            (not Resultable<std::invoke_result_t<ThenF, T&&>>) and
            (not std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>)
    constexpr auto Result<T, Err>::then(ThenF&& then_func) && noexcept
        -> Result<std::invoke_result_t<ThenF, T&&>, Err>
    {
        using R = Result<std::invoke_result_t<ThenF, T&&>, Err>;

        switch (tag)
        {
        case result_tag::ok:
        {
            return R{then_func(std::move(storage.ok))};
        }
        case result_tag::err:
        {
            return R{std::move(storage.err)};
        }
        }
    }

    template <typename T, typename Err>
    template <typename ThenF>
        requires
            std::is_nothrow_invocable_v<ThenF, T&&> and
            std::is_same_v<std::invoke_result_t<ThenF, T&&>, void>
    constexpr auto Result<T, Err>::then(ThenF&& then_func) && noexcept
        -> Result<Ok, Err>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            then_func(std::move(storage.ok));
            return Ok{};
        }
        case result_tag::err:
        {
            return std::move(storage.err);
        }
        }
    }

    template <typename T, typename Err>
    template <typename ThenF>
        requires
            std::is_nothrow_invocable_v<ThenF, T&&> and
            Resultable<std::invoke_result_t<ThenF, T&&>>
    constexpr auto Result<T, Err>::then(ThenF&& then_func) && noexcept
        -> Result
        <
            typename std::invoke_result_t<ThenF, T&&>::T,
            MergeUnique
            <
                Err,
                typename std::invoke_result_t<ThenF, T&&>::Err
            >
        >
    {
        using R = Result
        <
            typename std::invoke_result_t<ThenF, T&&>::T,
            MergeUnique
            <
                Err,
                typename std::invoke_result_t<ThenF, T&&>::Err
            >
        >;

        switch (tag)
        {
        case result_tag::ok:
        {
            return then_func(std::move(storage.ok))
                .match
                (
                    [] (auto&& okv) noexcept -> R
                    {
                        return R{std::move(okv)};
                    },
                    [] (auto&& err) noexcept -> R
                    {
                        return R{std::move(err)};
                    }
                );
        }
        case result_tag::err:
        {
            return R{std::move(storage.err)};
        }
        }
    }

    template <typename T, typename Err>
    template <typename OnErrorF>
        requires
            std::is_nothrow_invocable_v<OnErrorF, Err&&> and
            (not std::is_same_v<std::invoke_result_t<OnErrorF, Err&&>, void>)
    constexpr auto Result<T, Err>::on_error(OnErrorF&& on_error_func) && noexcept
        -> Result<T, std::invoke_result_t<OnErrorF, Err&&>>
    {
        switch (tag)
        {
        case result_tag::ok:
        {
            return std::move(storage.ok);
        }
        case result_tag::err:
        {
            return on_error_func(std::move(storage.err));
        }
        }
    }
}
