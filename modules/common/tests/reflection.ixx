module;

#include <gtest/gtest.h>

#include <unordered_map>

export module tests_reflection;

import reflection;

namespace
{
    struct Foo final
    {
        int bar;
        unsigned int baz;
    };

    constexpr auto register_members(const Foo*) noexcept
    {
        using namespace ge;
        return std::tuple
        {
            Member<&Foo::bar>{"bar"},
            Member<&Foo::baz>{"baz"}
        };
    }
}

TEST(reflection, for_each_member)
{
    using namespace ge;

    Foo object{-1, 2};

    std::unordered_map<std::string_view, int64_t> expected_values
    {
        {"bar", -1},
        {"baz", 2}
    };

    for_each_member<Foo>
    (
        [&object, &expected_values] (const auto& member)
        {
            const auto& value = member.get(object);
            const auto it = expected_values.find(member.name);
            ASSERT_NE(it, expected_values.end());
            EXPECT_EQ(it->second, value);
            expected_values.erase(it);

            member.set(object, -value);
        }
    );

    EXPECT_TRUE(expected_values.empty());

    EXPECT_EQ(1, object.bar);
    EXPECT_EQ(-2, object.baz);
}
