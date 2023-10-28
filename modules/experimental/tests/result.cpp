#include "ge/common/experimental/result.hpp"
#include "ge/common/experimental/vector.hpp"

#include <gtest/gtest.h>

#include <variant>

namespace
{
    using namespace ge;

    enum class FooError
    {
        First,
        Second
    };

    [[ nodiscard ]] Result
    <
        int,
        Errors
        <
            FooError
        >
    > foo() noexcept
    {
        return FooError::First;
    }

    enum class BarError
    {
        First,
        Second
    };

    [[ nodiscard ]] Result
    <
        float,
        Errors
        <
            BarError,
            Allocator::AllocationError
        >
    > bar(int) noexcept
    {
        return BarError::Second;
    }

    enum class BazError
    {
        First,
        Second
    };

    template <typename F, typename... Args>
    using InvokeErrors = typename std::invoke_result_t<F, Args...>::Err;

    [[ nodiscard ]] Option
    <
        Errors
        <
            Vector<float>::PushToReservedError
        >
    > fill_vector
    (
        Vector<float>& /*vec*/,
        const float /*value*/,
        const size_t /*count*/
    )
    {
//        for (size_t i = 0; i < count; ++i)
//        {
//            if (auto errors = vec.push_back_to_reserved(value); errors.is_some())
//            {
//                return errors;
//            }
//        }

        return none;
    }

    static Allocator allocator;

    [[ nodiscard ]] Result
    <
        Vector<float>,
        Errors
        <
            Allocator::AllocationError
        >
    > create_vector_with_value(const float v) noexcept
    {
        using R = Result<Vector<float>, Errors<Allocator::AllocationError>>;

        constexpr size_t count = 10;
        return Vector<float>::create(allocator, count)
            .match
            (
                [v] (Vector<float>&& vec) noexcept -> R
                {
                    [[ maybe_unused ]] auto error = fill_vector(vec, v, count);
                    assert(error.is_some());
                    return std::move(vec);
                },
                [] (auto&& err) noexcept -> R
                {
                    return std::move(err);
                }
            );
    }

    [[ nodiscard ]] Result
    <
        size_t,
        Errors
        <
            InvokeErrors<decltype(&foo)>,
            InvokeErrors<decltype(&bar), int>,
            BazError,
            Allocator::AllocationError
        >
    > baz() noexcept
    {
        using R = Result
        <
            size_t,
            Errors
            <
                InvokeErrors<decltype(&foo)>,
                InvokeErrors<decltype(&bar), int>,
                BazError,
                Allocator::AllocationError
            >
        >;

        return foo()
            .match
            (
                [] (int v) noexcept -> R
                {
                    return bar(v)
                        .match
                        (
                            [] (float v) noexcept -> R
                            {
                                return create_vector_with_value(v)
                                    .match
                                    (
                                        [] (Vector<float>&& vec) noexcept -> R
                                        {
                                            return vec.size();
                                        },
                                        [] (Errors<Allocator::AllocationError>&& err) noexcept -> R
                                        {
                                            return std::move(err);
                                        }
                                    );
                            },
                            [] (auto&& err) noexcept -> R
                            {
                                return std::move(err);
                            }
                        );
                },
                [] (auto&& err) noexcept -> R
                {
                    return std::move(err);
                }
            );
    }

    [[ nodiscard ]] Result
    <
        size_t,
        Errors
        <
            FooError,
            BarError,
            Allocator::AllocationError
        >
    > bazbaz() noexcept
    {
        return foo()
            .then(bar)
            .then(create_vector_with_value)
            .then
            (
                [] (Vector<float>&& vec) noexcept
                {
                    return vec.size();
                }
            );
    }
}

static_assert
(
    std::is_same_v
    <
        ge::MergeUnique<std::tuple<int, float>, std::tuple<int, char>>,
        std::tuple<int, float, char>
    >
);

TEST(errors, erros)
{
    [[ maybe_unused ]] auto b = baz();
    [[ maybe_unused ]] auto bb = bazbaz();
}
