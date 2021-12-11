#include "ge/common/experimental/either.hpp"

#include <gtest/gtest.h>

#include <optional>
#include <utility>

namespace
{
    int first_counter = 0;

    struct First final
    {
        explicit First(int v) noexcept
            : value{v}
        {
            ++first_counter;
        }

        ~First() noexcept
        {
            --first_counter;
        }

        First(First&& other) noexcept
            : value{std::move(other.value)}
        {
            ++first_counter;
            other.value.reset();
        }
        First& operator=(First&& other) noexcept
        {
            if (this != &other)
            {
                First(std::move(other)).swap(*this);
            }

            return *this;
        }

        First(const First&) = delete;
        First& operator=(const First&) = delete;

        void swap(First& other)
        {
            std::swap(value, other.value);
        }

        std::optional<int> value;
    };

    int second_counter = 0;
    struct Second final
    {
        explicit Second(float v) noexcept
            : value{v}
        {
            ++second_counter;
        }

        ~Second() noexcept
        {
            --second_counter;
        }

        Second(Second&& other) noexcept
            : value{std::move(other.value)}
        {
            ++second_counter;
            other.value.reset();
        }
        Second& operator=(Second&& other) noexcept
        {
            if (this != &other)
            {
                Second(std::move(other)).swap(*this);
            }

            return *this;
        }

        Second(const Second&) = delete;
        Second& operator=(const Second&) = delete;

        void swap(Second& other)
        {
            std::swap(value, other.value);
        }

        std::optional<float> value;
    };
}

TEST(either, construct_and_destruct)
{
    using namespace ge;

    constexpr Either<int, float> either_int(1);
    constexpr Either<int, float> either_float(1.f);

    // TODO: check in constexpr context

    first_counter = 0;
    second_counter = 0;
    {
        Either<First, Second> either{First{1}};
        EXPECT_EQ(1, first_counter);
        EXPECT_EQ(0, second_counter);
    }
    EXPECT_EQ(0, first_counter);
    EXPECT_EQ(0, second_counter);

    first_counter = 0;
    second_counter = 0;
    {
        Either<First, Second> either{Second{1}};
        EXPECT_EQ(0, first_counter);
        EXPECT_EQ(1, second_counter);
    }
    EXPECT_EQ(0, first_counter);
    EXPECT_EQ(0, second_counter);
}

TEST(either, match_no_return)
{
    using namespace ge;

    Either<First, Second> first{First{1}};

    first.match
    (
        [] (const First& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(1, *value.value);
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        }
    );

    const Either<First, Second> second{Second{2.f}};

    second.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        },
        [] (const Second& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(2.f, *value.value);
        }
    );

    constexpr Either<int, float> either_int(1);
    // TODO: check in constexpr context
    either_int.match
    (
        [] (const int& value) noexcept
        {
            EXPECT_EQ(1, value);
        },
        [] (const float&) noexcept
        {
            FAIL();
        }
    );
}

TEST(either, match_with_return)
{
    using namespace ge;

    Either<First, Second> first{First{1}};

    const bool first_result = first.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_TRUE(value.value.has_value());
            EXPECT_EQ(1, *value.value);
            return true;
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            return false;
        }
    );

    EXPECT_TRUE(first_result);

    const Either<First, Second> second{Second{2.f}};

    const bool second_result = second.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            return false;
        },
        [] (const Second& value) noexcept
        {
            EXPECT_TRUE(value.value.has_value());
            EXPECT_EQ(2.f, *value.value);
            return true;
        }
    );

    EXPECT_TRUE(second_result);
}

TEST(either, match_part)
{
    using namespace ge;

    Either<First, Second> first{First{1}};

    bool first_result = false;
    first.match_first
    (
        [&first_result] (const First& value) noexcept
        {
            EXPECT_TRUE(value.value.has_value());
            EXPECT_EQ(1, *value.value);
            first_result = true;
        }
    );

    EXPECT_TRUE(first_result);

    const Either<First, Second> second{Second{2.f}};

    bool second_result = false;
    second.match_second
    (
        [&second_result] (const Second& value) noexcept
        {
            EXPECT_TRUE(value.value.has_value());
            EXPECT_EQ(2.f, *value.value);
            second_result = true;
            return;
        }
    );

    EXPECT_TRUE(second_result);
}

TEST(either, swap_diff_types)
{
    using namespace ge;

    first_counter = 0;
    second_counter = 0;

    Either<First, Second> first{First{1}};
    Either<First, Second> second{Second{2.f}};

    std::swap(first, second);

    EXPECT_EQ(1, first_counter);
    EXPECT_EQ(1, second_counter);

    first.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        },
        [] (const Second& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(2.f, *value.value);
        }
    );

    second.match
    (
        [] (const First& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(1, *value.value);
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        }
    );
}

TEST(either, swap_same_types)
{
    using namespace ge;

    Either<First, Second> first{First{1}};
    Either<First, Second> second{First{2}};

    std::swap(first, second);

    first.match
    (
        [] (const First& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(2, *value.value);
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        }
    );

    second.match
    (
        [] (const First& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(1, *value.value);
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        }
    );
}

TEST(either, move_assign)
{
    using namespace ge;

    first_counter = 0;
    second_counter = 0;

    Either<First, Second> first{First{1}};
    Either<First, Second> second{Second{2.f}};
    first = std::move(second);

    EXPECT_EQ(0, first_counter);
    EXPECT_EQ(2, second_counter);

    first.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        },
        [] (const Second& value) noexcept
        {
            ASSERT_TRUE(value.value.has_value());
            EXPECT_EQ(2.f, *value.value);
        }
    );

    second.match
    (
        [] (const First& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
            FAIL();
        },
        [] (const Second& value) noexcept
        {
            EXPECT_FALSE(value.value.has_value());
        }
    );

    // TODO: check in constexpr context
}

TEST(either, is)
{
    using namespace ge;

    constexpr Either<int, float> either_int(1);
    static_assert(either_int.is_first());
    static_assert(not either_int.is_second());

    constexpr Either<int, float> either_float(2.f);
    static_assert(not either_float.is_first());
    static_assert(either_float.is_second());
}

TEST(either, reference)
{
    using namespace ge;

    int i = 1;
    float f = 2.f;

    {
        Either<Ref<int>, Ref<float>> either_int(i);
        Either<Ref<int>, Ref<float>> either_float(f);

        either_int.match
        (
            [] (int& v) noexcept
            {
                EXPECT_EQ(1, v);
            },
            [] (float&) noexcept
            {
                FAIL();
            }
        );

        {
            bool result = false;
            either_int.match_first
            (
                [&result] (int& v) noexcept
                {
                    EXPECT_EQ(1, v);
                    result = true;
                }
            );
            EXPECT_TRUE(result);
        }
        {
            bool result = false;
            either_float.match_second
            (
                [&result] (float& v) noexcept
                {
                    EXPECT_EQ(2.f, v);
                    result = true;
                }
            );
            EXPECT_TRUE(result);
        }
        {
            const bool result = either_int.match
            (
                [] (int&) noexcept
                {
                    return true;
                },
                [] (float&) noexcept
                {
                    return false;
                }
            );
            EXPECT_TRUE(result);
        }

        std::swap(either_int, either_float);
        either_int = std::move(either_float);
        Either<Ref<int>, Ref<float>> either_new(std::move(either_int));
        EXPECT_TRUE(either_new.is_second());
        either_new.match_second
        (
            [] (float& v) noexcept
            {
                EXPECT_EQ(2.f, v);
            }
        );
    }
    {
        Either<Ref<const int>, Ref<const float>> either_int(i);
        Either<Ref<const int>, Ref<const float>> either_float(f);

        either_int.match
        (
            [] (const int& v) noexcept
            {
                EXPECT_EQ(1, v);
            },
            [] (const float&) noexcept
            {
                FAIL();
            }
        );
    }
    {
        const Either<Ref<int>, Ref<float>> either_int(i);
        const Either<Ref<int>, Ref<float>> either_float(f);

        either_int.match
        (
            [] (int& v) noexcept
            {
                EXPECT_EQ(1, v);
            },
            [] (float&) noexcept
            {
                FAIL();
            }
        );
    }
}
