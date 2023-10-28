#include "ge/common/experimental/variant.hpp"

#include <gtest/gtest.h>

TEST(variant, construct)
{
    using namespace ge;

    {
        Variant<int, float, char> val(1);
        val.match
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
        Variant<int, float, char> val(2.f);
        val.match
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
        Variant<int, float, char> val('a');
        val.match
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
