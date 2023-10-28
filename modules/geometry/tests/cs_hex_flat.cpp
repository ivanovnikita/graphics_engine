#include "ge/geometry/cs_hex_flat.hpp"

#include <gtest/gtest.h>

TEST(cs_hex_flat, draw_space_to_hex_doubled_height)
{
    using namespace ge;

    const CsHexFlat cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -2.1f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -2.9f};
        const HexCoordDoubledHeight ph{0, -2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -0.1f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -1.9f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -1.9f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -2.1f};
        const HexCoordDoubledHeight ph{-1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -0.1f};
        const HexCoordDoubledHeight ph{-2, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -3.9f};
        const HexCoordDoubledHeight ph{-2, -2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 2.1f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 2.9f};
        const HexCoordDoubledHeight ph{0, 2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 0.1f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 1.9f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 1.9f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 2.1f};
        const HexCoordDoubledHeight ph{-1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 0.1f};
        const HexCoordDoubledHeight ph{-2, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 3.9f};
        const HexCoordDoubledHeight ph{-2, 2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -2.1f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -2.9f};
        const HexCoordDoubledHeight ph{0, -2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -0.1f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -1.9f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -1.9f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -2.1f};
        const HexCoordDoubledHeight ph{1, -1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -0.1f};
        const HexCoordDoubledHeight ph{2, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -3.9f};
        const HexCoordDoubledHeight ph{2, -2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 2.1f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const HexCoordDoubledHeight ph{0, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 2.9f};
        const HexCoordDoubledHeight ph{0, 2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 0.1f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 1.9f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 1.9f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 2.1f};
        const HexCoordDoubledHeight ph{1, 1};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 0.1f};
        const HexCoordDoubledHeight ph{2, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 3.9f};
        const HexCoordDoubledHeight ph{2, 2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }

    // regression
    {
        const Point2dF p{4.81625f, 0.936103f};
        const HexCoordDoubledHeight ph{2, 0};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.81625f, 2.936103f};
        const HexCoordDoubledHeight ph{2, 2};
        const HexCoordDoubledHeight phr = cs.to_hex_doubled_height(p);
        EXPECT_EQ(ph, phr);
    }
}

TEST(cs_hex_flat, hex_doubled_height_to_draw_space)
{
    using namespace ge;

    const CsHexFlat cs{4.f, 4.f, -1.f, 1.f};

    {
        const HexCoordDoubledHeight ph{0, 0};
        const Point2dF p{0.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{2, 0};
        const Point2dF p{6.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{2, 2};
        const Point2dF p{6.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{2, -2};
        const Point2dF p{6.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{-2, 0};
        const Point2dF p{-6.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{-2, 2};
        const Point2dF p{-6.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{-2, -2};
        const Point2dF p{-6.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{1, 1};
        const Point2dF p{3.f, 2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{1, 3};
        const Point2dF p{3.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{1, -1};
        const Point2dF p{3.f, -2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{1, -3};
        const Point2dF p{3.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{-1, 1};
        const Point2dF p{-3.f, 2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{-1, 3};
        const Point2dF p{-3.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{-1, -1};
        const Point2dF p{-3.f, -2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{-1, -3};
        const Point2dF p{-3.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{0, 2};
        const Point2dF p{0.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{0, 4};
        const Point2dF p{0.f, 8.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledHeight ph{0, -2};
        const Point2dF p{0.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledHeight ph{0, -4};
        const Point2dF p{0.f, -8.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
}

TEST(cs_hex_flat, hex_doubled_height_to_offset_flat)
{
    using namespace ge;

    {
        const HexCoordOffsetFlat e{0, 0};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetFlat e{1, 0};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{1, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-1, 0};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-1, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetFlat e{2, 1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{2, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-2, 1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-2, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordDoubledHeight{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, offset_flat_to_hex_doubled_height)
{
    using namespace ge;

    {
        const HexCoordDoubledHeight e{0, 0};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledHeight e{1, 3};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{1, -1};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-1, 3};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-1, -1};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledHeight e{2, 4};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{2, -4};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-2, 4};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-2, -4};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordOffsetFlat{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, axial_flat_to_offset_flat)
{
    using namespace ge;

    {
        const HexCoordOffsetFlat e{0, 0};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetFlat e{1, 1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{1, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-1, 0};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-1, -2};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetFlat e{2, 3};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{2, -1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-2, 1};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetFlat e{-2, -3};
        const HexCoordOffsetFlat r = to_hex_offset_flat(HexCoordAxialFlat{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, offset_flat_to_axial_flat)
{
    using namespace ge;

    {
        const HexCoordAxialFlat e{0, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat e{1, 1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{1, -1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-1, 2};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-1, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat e{2, 1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{2, -3};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-2, 3};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-2, -1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordOffsetFlat{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, axial_flat_to_hex_doubled_height)
{
    using namespace ge;

    {
        const HexCoordDoubledHeight e{0, 0};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledHeight e{1, 3};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{1, -1};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-1, 1};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-1, -3};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledHeight e{2, 6};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{2, -2};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-2, 2};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledHeight e{-2, -6};
        const HexCoordDoubledHeight r = to_hex_doubled_height(HexCoordAxialFlat{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, hex_doubled_height_to_axial_flat)
{
    using namespace ge;

    {
        const HexCoordAxialFlat e{0, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat e{1, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{1, -1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-1, 1};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-1, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat e{2, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{2, -2};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-2, 2};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialFlat e{-2, 0};
        const HexCoordAxialFlat r = to_hex_axial_flat(HexCoordDoubledHeight{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_flat, draw_space_to_hex_axial_flat)
{
    using namespace ge;

    const CsHexFlat cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -2.1f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, -2.9f};
        const HexCoordAxialFlat ph{0, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -0.1f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, -1.9f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -1.9f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, -2.1f};
        const HexCoordAxialFlat ph{-1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -0.1f};
        const HexCoordAxialFlat ph{-2, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, -3.9f};
        const HexCoordAxialFlat ph{-2, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 2.1f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.1f, 2.9f};
        const HexCoordAxialFlat ph{0, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 0.1f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.f, 1.9f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 1.9f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.1f, 2.1f};
        const HexCoordAxialFlat ph{-1, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 0.1f};
        const HexCoordAxialFlat ph{-2, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-4.9f, 3.9f};
        const HexCoordAxialFlat ph{-2, 2};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -2.1f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, -2.9f};
        const HexCoordAxialFlat ph{0, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -0.1f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, -1.9f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -1.9f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, -2.1f};
        const HexCoordAxialFlat ph{1, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -0.1f};
        const HexCoordAxialFlat ph{2, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, -3.9f};
        const HexCoordAxialFlat ph{2, -2};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 2.1f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const HexCoordAxialFlat ph{0, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.1f, 2.9f};
        const HexCoordAxialFlat ph{0, 1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 0.1f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.f, 1.9f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 1.9f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.1f, 2.1f};
        const HexCoordAxialFlat ph{1, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 0.1f};
        const HexCoordAxialFlat ph{2, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.9f, 3.9f};
        const HexCoordAxialFlat ph{2, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }

    // regression
    {
        const Point2dF p{4.81625f, 0.936103f};
        const HexCoordAxialFlat ph{2, -1};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{4.81625f, 2.936103f};
        const HexCoordAxialFlat ph{2, 0};
        const HexCoordAxialFlat phr = cs.to_hex_axial_flat(p);
        EXPECT_EQ(ph, phr);
    }
}

TEST(cs_hex_flat, hex_axial_flat_to_draw_space)
{
    using namespace ge;

    const CsHexFlat cs{4.f, 4.f, -1.f, 1.f};

    {
        const HexCoordAxialFlat ph{0, 0};
        const Point2dF p{0.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{2, -1};
        const Point2dF p{6.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{2, 0};
        const Point2dF p{6.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{2, -2};
        const Point2dF p{6.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{-2, 1};
        const Point2dF p{-6.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{-2, 2};
        const Point2dF p{-6.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{-2, 0};
        const Point2dF p{-6.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{1, 0};
        const Point2dF p{3.f, 2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{1, 1};
        const Point2dF p{3.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{1, -1};
        const Point2dF p{3.f, -2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{1, -2};
        const Point2dF p{3.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{-1, 1};
        const Point2dF p{-3.f, 2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{-1, 2};
        const Point2dF p{-3.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{-1, 0};
        const Point2dF p{-3.f, -2.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{-1, -1};
        const Point2dF p{-3.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{0, 1};
        const Point2dF p{0.f, 4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{0, 2};
        const Point2dF p{0.f, 8.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialFlat ph{0, -1};
        const Point2dF p{0.f, -4.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialFlat ph{0, -2};
        const Point2dF p{0.f, -8.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
}
