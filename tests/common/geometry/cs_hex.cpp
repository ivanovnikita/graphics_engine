#include "ge/geometry/cs_hex.hpp"

#include <gtest/gtest.h>

TEST(cs_hex, inside)
{
    using namespace ge;

    const CsHex cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -2.1f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -2.9f};
        const CoordHex ph{0, -2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -0.1f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -1.9f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -1.9f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -2.1f};
        const CoordHex ph{-1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -0.1f};
        const CoordHex ph{-2, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -3.9f};
        const CoordHex ph{-2, -2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 2.1f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 2.9f};
        const CoordHex ph{0, 2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 0.1f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 1.9f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 1.9f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 2.1f};
        const CoordHex ph{-1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 0.1f};
        const CoordHex ph{-2, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 3.9f};
        const CoordHex ph{-2, 2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -2.1f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -2.9f};
        const CoordHex ph{0, -2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -0.1f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -1.9f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -1.9f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -2.1f};
        const CoordHex ph{1, -1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -0.1f};
        const CoordHex ph{2, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -3.9f};
        const CoordHex ph{2, -2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 2.1f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const CoordHex ph{0, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 2.9f};
        const CoordHex ph{0, 2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 0.1f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 1.9f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 1.9f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 2.1f};
        const CoordHex ph{1, 1};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 0.1f};
        const CoordHex ph{2, 0};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 3.9f};
        const CoordHex ph{2, 2};
        const CoordHex phr = cs.convert(p);
        EXPECT_EQ(ph, phr);
    }
}
