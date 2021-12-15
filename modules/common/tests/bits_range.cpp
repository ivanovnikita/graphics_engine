#include "ge/common/bits_range.hpp"

#include <gtest/gtest.h>

TEST(bits_range, deref)
{
    using namespace ge;

    const uint8_t i = 0b00001010;

    {
        const BitsIterator it(i, 0);
        EXPECT_EQ(0b00000000, *it);
    }
    {
        const BitsIterator it(i, 1);
        EXPECT_EQ(0b00000010, *it);
    }
    {
        const BitsIterator it(i, 2);
        EXPECT_EQ(0b00000000, *it);
    }
    {
        const BitsIterator it(i, 3);
        EXPECT_EQ(0b00001000, *it);
    }
}

TEST(bits_range, increment)
{
    using namespace ge;

    const uint8_t i = 0b00001010;

    BitsIterator it(i);
    EXPECT_EQ(0b00000000, *it);

    ++it;
    EXPECT_EQ(0b00000010, *it);

    ++it;
    EXPECT_EQ(0b00000000, *it);

    it++;
    EXPECT_EQ(0b00001000, *it);

    it++;
    EXPECT_EQ(0b00000000, *it);
}

TEST(bits_range, decrement)
{
    using namespace ge;

    const uint8_t i = 0b00001010;

    BitsIterator it(i, 4);
    EXPECT_EQ(0b00000000, *it);

    --it;
    EXPECT_EQ(0b00001000, *it);

    --it;
    EXPECT_EQ(0b00000000, *it);

    it--;
    EXPECT_EQ(0b00000010, *it);

    it--;
    EXPECT_EQ(0b00000000, *it);
}

TEST(bits_range, range)
{
    using namespace ge;

    const uint8_t i = 0b00001010;

    const auto bits_range = to_bits_range(i);

    std::vector<uint8_t> bits_from_range;
    bits_from_range.reserve(8 * sizeof(uint8_t));

    std::ranges::copy(bits_range, std::back_inserter(bits_from_range));

    const std::vector<uint8_t> expected_range
    {
        0b00000000,
        0b00000010,
        0b00000000,
        0b00001000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000
    };

    EXPECT_EQ(expected_range, bits_from_range);
}
