#include "ge/common/option.hpp"

#include <gtest/gtest.h>

TEST(option, some)
{
    using namespace ge;

    const auto func = []
    {
        return some(1);
    };

    const Option<int> result = func();
    EXPECT_TRUE(result.is_some());
    EXPECT_FALSE(result.is_none());
}

TEST(option, none)
{
    using namespace ge;

    const auto func = []
    {
        return none;
    };

    const Option<int> result = func();
    EXPECT_FALSE(result.is_some());
    EXPECT_TRUE(result.is_none());
}

TEST(option, match)
{
    using namespace ge;

    Option<int> result(1);

    result.match
    (
        [] (int v) noexcept
        {
            EXPECT_EQ(1, v);
        },
        [] (None) noexcept
        {
            FAIL();
        }
    );
}

TEST(option, move_construct)
{
    using namespace ge;

    Option<int> one(1);
    const Option<int> two(std::move(one));

    EXPECT_TRUE(one.is_none());
    EXPECT_TRUE(two.is_some());

    two.match
    (
        [] (int v) noexcept
        {
            EXPECT_EQ(1, v);
        },
        [] (None) noexcept
        {
            FAIL();
        }
    );
}

TEST(option, move_assign)
{
    using namespace ge;

    Option<int> one(1);
    Option<int> two(2);
    two = std::move(one);

    EXPECT_TRUE(one.is_none());
    EXPECT_TRUE(two.is_some());

    two.match
    (
        [] (int v) noexcept
        {
            EXPECT_EQ(1, v);
        },
        [] (None) noexcept
        {
            FAIL();
        }
    );
}

TEST(option, reset)
{
    using namespace ge;

    Option<int> one(1);
    one = none;

    EXPECT_TRUE(one.is_none());
}
