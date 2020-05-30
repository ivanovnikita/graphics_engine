#include <ge/render/factory/tools.hpp>

#include <gtest/gtest.h>

TEST(tools, not_contained_in)
{
    using namespace ge::factory;

    constexpr std::array what{1, 2, 3, 4};
    constexpr std::array where{2, 4};
    constexpr std::array expected_result{1, 3};

    RangeView<int> auto result = not_contained_in(what, where);

    size_t ind = 0;
    for (const auto& result_element : result)
    {
        EXPECT_EQ(expected_result[ind], result_element);
        ++ind;
    }
    EXPECT_NE(0, ind);
}
