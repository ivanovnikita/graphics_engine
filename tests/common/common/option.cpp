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

TEST(option, match_no_return)
{
    using namespace ge;

    Option<int> result(1);

    result.match
    (
        [] (int v) noexcept
        {
            EXPECT_EQ(1, v);
        },
        [] () noexcept
        {
            FAIL();
        }
    );
}

TEST(option, match_with_return)
{
    using namespace ge;

    Option<int> option(1);

    const bool result = option.match
    (
        [] (int v) noexcept
        {
            EXPECT_EQ(1, v);
            return true;
        },
        [] () noexcept
        {
            return false;
        }
    );

    EXPECT_TRUE(result);
}

TEST(option, match_some)
{
    using namespace ge;

    Option<int> option(1);

    bool result = false;
    option.match_some
    (
        [&result] (int v) noexcept
        {
            EXPECT_EQ(1, v);
            result = true;
        }
    );

    EXPECT_TRUE(result);
}

TEST(option, match_none)
{
    using namespace ge;

    Option<int> option(none);

    bool result = false;
    option.match_none
    (
        [&result] () noexcept
        {
            result = true;
        }
    );

    EXPECT_TRUE(result);
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
        [] () noexcept
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
        [] () noexcept
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
