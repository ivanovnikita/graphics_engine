#include "ge/common/variant.hpp"

#include <gtest/gtest.h>

TEST(variant, construct)
{
    using namespace ge;

    {
        Variant<int, float, char> v(1);
        v.match
        (
            [] (int v)
            {
                EXPECT_EQ(1, v);
            },
            [] (auto)
            {
                FAIL();
            }
        );
    }
    {
        Variant<int, float, char> v(2.f);
        v.match
        (
            [] (float v)
            {
                EXPECT_EQ(2.f, v);
            },
            [] (auto)
            {
                FAIL();
            }
        );
    }
    {
        Variant<int, float, char> v('a');
        v.match
        (
            [] (char v)
            {
                EXPECT_EQ('a', v);
            },
            [] (auto)
            {
                FAIL();
            }
        );
    }
}
