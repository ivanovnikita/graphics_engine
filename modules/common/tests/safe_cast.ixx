module;

#include <gtest/gtest.h>

export module tests_safe_cast;

import exception;
import safe_cast;

template<typename T, typename U>
struct CastPair
{
    using From = T;
    using To = U;

    static_assert(std::is_unsigned<From>::value && std::is_unsigned<To>::value, "Types must be unsigned");
};

template <class T>
class safe_castInt : public testing::Test
{
};

using AllPairsInt = testing::Types
<
    CastPair<uint8_t, uint8_t>
    , CastPair<uint8_t, uint16_t>
    , CastPair<uint8_t, uint32_t>
    , CastPair<uint8_t, uint64_t>

    , CastPair<uint16_t, uint8_t>
    , CastPair<uint16_t, uint16_t>
    , CastPair<uint16_t, uint32_t>
    , CastPair<uint16_t, uint64_t>

    , CastPair<uint32_t, uint8_t>
    , CastPair<uint32_t, uint16_t>
    , CastPair<uint32_t, uint32_t>
    , CastPair<uint32_t, uint64_t>

    , CastPair<uint64_t, uint8_t>
    , CastPair<uint64_t, uint16_t>
    , CastPair<uint64_t, uint32_t>
    , CastPair<uint64_t, uint64_t>
>;

TYPED_TEST_SUITE(safe_castInt, AllPairsInt);

TYPED_TEST(safe_castInt, unsignedToUnsigned)
{
    using namespace ge;

    using FromUnsigned = typename TypeParam::From;
    using ToUnsigned = typename TypeParam::To;

    const auto fromMaxUnsigned = std::numeric_limits<FromUnsigned>::max();

    if (sizeof(FromUnsigned) <= sizeof(ToUnsigned))
    {
        EXPECT_EQ(static_cast<ToUnsigned>(fromMaxUnsigned), safe_cast<ToUnsigned>(fromMaxUnsigned));
    }
    else
    {
        EXPECT_THROW(safe_cast<ToUnsigned>(fromMaxUnsigned), unexpected_error);
    }
}

TYPED_TEST(safe_castInt, unsignedToSigned)
{
    using namespace ge;

    using FromUnsigned = typename TypeParam::From;
    using ToSigned = typename std::make_signed<typename TypeParam::To>::type;

    const auto fromMaxUnsigned = std::numeric_limits<FromUnsigned>::max();

    if (sizeof(FromUnsigned) < sizeof(ToSigned))
    {
        EXPECT_EQ(static_cast<ToSigned>(fromMaxUnsigned), safe_cast<ToSigned>(fromMaxUnsigned));
    }
    else
    {
        EXPECT_THROW(safe_cast<ToSigned>(fromMaxUnsigned), unexpected_error);
    }
}

TYPED_TEST(safe_castInt, signedToUnsigned)
{
    using namespace ge;

    using FromSigned = typename std::make_signed<typename TypeParam::From>::type;
    using ToUnsigned = typename TypeParam::To;

    const auto fromMaxSigned = std::numeric_limits<FromSigned>::max();
    const auto fromMinSigned = std::numeric_limits<FromSigned>::min();

    if (sizeof(FromSigned) <= sizeof(ToUnsigned))
    {
        EXPECT_EQ(static_cast<ToUnsigned>(fromMaxSigned), safe_cast<ToUnsigned>(fromMaxSigned));
    }
    else
    {
        EXPECT_THROW(safe_cast<ToUnsigned>(fromMaxSigned), unexpected_error);
    }

    EXPECT_THROW(safe_cast<ToUnsigned>(fromMinSigned), unexpected_error);
}

TYPED_TEST(safe_castInt, signedToSigned)
{
    using namespace ge;

    using FromSigned = typename std::make_signed<typename TypeParam::From>::type;
    using ToSigned = typename std::make_signed<typename TypeParam::To>::type;

    const auto fromMaxSigned = std::numeric_limits<FromSigned>::max();
    const auto fromMinSigned = std::numeric_limits<FromSigned>::min();

    if (sizeof(FromSigned) <= sizeof(ToSigned))
    {
        EXPECT_EQ(static_cast<ToSigned>(fromMinSigned), safe_cast<ToSigned>(fromMinSigned));
        EXPECT_EQ(static_cast<ToSigned>(fromMaxSigned), safe_cast<ToSigned>(fromMaxSigned));
    }
    else
    {
        EXPECT_THROW(safe_cast<ToSigned>(fromMinSigned), unexpected_error);
        EXPECT_THROW(safe_cast<ToSigned>(fromMaxSigned), unexpected_error);
    }
}
