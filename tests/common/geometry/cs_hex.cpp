#include "ge/geometry/cs_hex.hpp"

#include <gtest/gtest.h>

TEST(cs_hex, to_hex)
{
    using namespace ge;

    const CsHex cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -2.1f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -2.9f};
        const HexCoordDoubled ph{0, -2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -0.1f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -1.9f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -1.9f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -2.1f};
        const HexCoordDoubled ph{-1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -0.1f};
        const HexCoordDoubled ph{-2, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -3.9f};
        const HexCoordDoubled ph{-2, -2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 2.1f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 2.9f};
        const HexCoordDoubled ph{0, 2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 0.1f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 1.9f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 1.9f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 2.1f};
        const HexCoordDoubled ph{-1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 0.1f};
        const HexCoordDoubled ph{-2, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 3.9f};
        const HexCoordDoubled ph{-2, 2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -2.1f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -2.9f};
        const HexCoordDoubled ph{0, -2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -0.1f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -1.9f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -1.9f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -2.1f};
        const HexCoordDoubled ph{1, -1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -0.1f};
        const HexCoordDoubled ph{2, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -3.9f};
        const HexCoordDoubled ph{2, -2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 2.1f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const HexCoordDoubled ph{0, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 2.9f};
        const HexCoordDoubled ph{0, 2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 0.1f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 1.9f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 1.9f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 2.1f};
        const HexCoordDoubled ph{1, 1};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 0.1f};
        const HexCoordDoubled ph{2, 0};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 3.9f};
        const HexCoordDoubled ph{2, 2};
        const HexCoordDoubled phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
}

TEST(cs_hex, from_hex)
{
    using namespace ge;

    const CsHex cs{4.f, 4.f, -1.f, 1.f};

    {
        const HexCoordDoubled ph{0, 0};
        const Point2dF p{0.f, 0.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{2, 0};
        const Point2dF p{6.f, 0.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{2, 2};
        const Point2dF p{6.f, 4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{2, -2};
        const Point2dF p{6.f, -4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{-2, 0};
        const Point2dF p{-6.f, 0.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{-2, 2};
        const Point2dF p{-6.f, 4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{-2, -2};
        const Point2dF p{-6.f, -4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{1, 1};
        const Point2dF p{3.f, 2.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{1, 3};
        const Point2dF p{3.f, 6.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{1, -1};
        const Point2dF p{3.f, -2.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{1, -3};
        const Point2dF p{3.f, -6.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{-1, 1};
        const Point2dF p{-3.f, 2.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{-1, 3};
        const Point2dF p{-3.f, 6.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{-1, -1};
        const Point2dF p{-3.f, -2.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{-1, -3};
        const Point2dF p{-3.f, -6.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{0, 2};
        const Point2dF p{0.f, 4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{0, 4};
        const Point2dF p{0.f, 8.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubled ph{0, -2};
        const Point2dF p{0.f, -4.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubled ph{0, -4};
        const Point2dF p{0.f, -8.f};
        const Point2dF pr = cs.convert(ph);
        EXPECT_EQ(p, pr);
    }
}
