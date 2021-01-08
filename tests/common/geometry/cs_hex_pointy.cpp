#include "ge/geometry/cs_hex_pointy.hpp"

#include <gtest/gtest.h>

TEST(cs_hex_pointy, draw_space_to_hex_doubled_width)
{
    using namespace ge;

    const CsHexPointy cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, -1.9f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -1.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.9f, -1.1f};
        const HexCoordDoubledWidth ph{-2, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -3.0f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -3.0f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -4.1f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, -4.1f};
        const HexCoordDoubledWidth ph{-1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -4.9f};
        const HexCoordDoubledWidth ph{0, -2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.9f, -4.9f};
        const HexCoordDoubledWidth ph{-2, -2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, 1.9f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 1.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.9f, 1.1f};
        const HexCoordDoubledWidth ph{-2, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 3.0f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 3.0f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 4.1f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, 4.1f};
        const HexCoordDoubledWidth ph{-1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 4.9f};
        const HexCoordDoubledWidth ph{0, 2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.9f, 4.9f};
        const HexCoordDoubledWidth ph{-2, 2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, -1.9f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -1.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.9f, -1.1f};
        const HexCoordDoubledWidth ph{2, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -3.0f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -3.0f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -4.1f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, -4.1f};
        const HexCoordDoubledWidth ph{1, -1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -4.9f};
        const HexCoordDoubledWidth ph{0, -2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.9f, -4.9f};
        const HexCoordDoubledWidth ph{2, -2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, 1.9f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 1.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const HexCoordDoubledWidth ph{0, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.9f, 1.1f};
        const HexCoordDoubledWidth ph{2, 0};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 3.0f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 3.0f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 4.1f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, 4.1f};
        const HexCoordDoubledWidth ph{1, 1};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 4.9f};
        const HexCoordDoubledWidth ph{0, 2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.9f, 4.9f};
        const HexCoordDoubledWidth ph{2, 2};
        const HexCoordDoubledWidth phr = cs.to_hex_doubled_width(p);
        EXPECT_EQ(ph, phr);
    }
}

TEST(cs_hex_pointy, hex_doubled_width_to_draw_space)
{
    using namespace ge;

    const CsHexPointy cs{4.f, 4.f, -1.f, 1.f};

    {
        const HexCoordDoubledWidth ph{0, 0};
        const Point2dF p{0.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{2, 0};
        const Point2dF p{4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{2, 2};
        const Point2dF p{4.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{2, -2};
        const Point2dF p{4.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{-2, 0};
        const Point2dF p{-4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{-2, 2};
        const Point2dF p{-4.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{-2, -2};
        const Point2dF p{-4.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{1, 1};
        const Point2dF p{2.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{3, 1};
        const Point2dF p{6.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{1, -1};
        const Point2dF p{2.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{3, -1};
        const Point2dF p{6.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{-1, 1};
        const Point2dF p{-2.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{-3, 1};
        const Point2dF p{-6.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{-1, -1};
        const Point2dF p{-2.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{-3, -1};
        const Point2dF p{-6.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{2, 0};
        const Point2dF p{4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{4, 0};
        const Point2dF p{8.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordDoubledWidth ph{-2, 0};
        const Point2dF p{-4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordDoubledWidth ph{-4, 0};
        const Point2dF p{-8.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
}

TEST(cs_hex_pointy, hex_doubled_width_to_offset_pointy)
{
    using namespace ge;

    {
        const HexCoordOffsetPointy e{0, 0};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetPointy e{0, 1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{0, -1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, 1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, -1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetPointy e{1, 2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{1, -2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, 2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, -2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordDoubledWidth{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, offset_pointy_to_hex_doubled_width)
{
    using namespace ge;

    {
        const HexCoordDoubledWidth e{0, 0};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledWidth e{3, 1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{3, -1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-1, 1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-1, -1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledWidth e{4, 2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{4, -2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-4, 2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-4, -2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordOffsetPointy{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, axial_pointy_to_offset_pointy)
{
    using namespace ge;

    {
        const HexCoordOffsetPointy e{0, 0};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetPointy e{1, 1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{0, -1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, 1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-2, -1};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordOffsetPointy e{3, 2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{1, -2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-1, 2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordOffsetPointy e{-3, -2};
        const HexCoordOffsetPointy r = to_hex_offset_pointy(HexCoordAxialPointy{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, offset_pointy_to_axial_pointy)
{
    using namespace ge;

    {
        const HexCoordAxialPointy e{0, 0};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy e{1, 1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{2, -1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{-1, 1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{0, -1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy e{1, 2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{3, -2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{-3, 2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{-1, -2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordOffsetPointy{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, axial_pointy_to_hex_doubled_width)
{
    using namespace ge;

    {
        const HexCoordDoubledWidth e{0, 0};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledWidth e{3, 1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{1, -1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-1, 1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-3, -1};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordDoubledWidth e{6, 2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{2, -2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-2, 2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordDoubledWidth e{-6, -2};
        const HexCoordDoubledWidth r = to_hex_doubled_width(HexCoordAxialPointy{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, hex_doubled_width_to_axial_pointy)
{
    using namespace ge;

    {
        const HexCoordAxialPointy e{0, 0};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{0, 0});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy e{0, 1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{1, -1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{1, -1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{-1, 1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{-1, 1});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{0, -1};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{-1, -1});
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy e{0, 2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{2, -2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{2, -2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{-2, 2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{-2, 2});
        EXPECT_EQ(e, r);
    }
    {
        const HexCoordAxialPointy e{0, -2};
        const HexCoordAxialPointy r = to_hex_axial_pointy(HexCoordDoubledWidth{-2, -2});
        EXPECT_EQ(e, r);
    }
}

TEST(cs_hex_pointy, draw_space_to_hex_axial_pointy)
{
    using namespace ge;

    const CsHexPointy cs{4.f, 4.f, -1.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }

    {
        const Point2dF p{-1.9f, -1.9f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, -1.9f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -1.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -0.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.9f, -1.1f};
        const HexCoordAxialPointy ph{-1, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -3.0f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -3.0f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, -4.1f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, -4.1f};
        const HexCoordAxialPointy ph{0, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, -4.9f};
        const HexCoordAxialPointy ph{1, -2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.9f, -4.9f};
        const HexCoordAxialPointy ph{0, -2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{-1.9f, 1.9f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, 1.9f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 1.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 0.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.9f, 1.1f};
        const HexCoordAxialPointy ph{-1, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 3.0f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 3.0f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-1.9f, 4.1f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-2.1f, 4.1f};
        const HexCoordAxialPointy ph{-1, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-0.1f, 4.9f};
        const HexCoordAxialPointy ph{-1, 2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{-3.9f, 4.9f};
        const HexCoordAxialPointy ph{-2, 2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, -1.9f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, -1.9f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -1.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -0.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.9f, -1.1f};
        const HexCoordAxialPointy ph{1, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -3.0f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -3.0f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, -4.1f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, -4.1f};
        const HexCoordAxialPointy ph{1, -1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, -4.9f};
        const HexCoordAxialPointy ph{1, -2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.9f, -4.9f};
        const HexCoordAxialPointy ph{2, -2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }


    {
        const Point2dF p{1.9f, 1.9f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, 1.9f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 1.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 0.1f};
        const HexCoordAxialPointy ph{0, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.9f, 1.1f};
        const HexCoordAxialPointy ph{1, 0};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 3.0f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 3.0f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{1.9f, 4.1f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{2.1f, 4.1f};
        const HexCoordAxialPointy ph{0, 1};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{0.1f, 4.9f};
        const HexCoordAxialPointy ph{-1, 2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
    {
        const Point2dF p{3.9f, 4.9f};
        const HexCoordAxialPointy ph{0, 2};
        const HexCoordAxialPointy phr = cs.to_hex_axial_pointy(p);
        EXPECT_EQ(ph, phr);
    }
}

TEST(cs_hex_pointy, hex_axial_pointy_to_draw_space)
{
    using namespace ge;

    const CsHexPointy cs{4.f, 4.f, -1.f, 1.f};

    {
        const HexCoordAxialPointy ph{0, 0};
        const Point2dF p{0.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{1, 0};
        const Point2dF p{4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{0, 2};
        const Point2dF p{4.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{2, -2};
        const Point2dF p{4.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{-1, 0};
        const Point2dF p{-4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{-2, 2};
        const Point2dF p{-4.f, 6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{0, -2};
        const Point2dF p{-4.f, -6.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{0, 1};
        const Point2dF p{2.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{1, 1};
        const Point2dF p{6.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{1, -1};
        const Point2dF p{2.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{2, -1};
        const Point2dF p{6.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{-1, 1};
        const Point2dF p{-2.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{-2, 1};
        const Point2dF p{-6.f, 3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{0, -1};
        const Point2dF p{-2.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{-1, -1};
        const Point2dF p{-6.f, -3.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{1, 0};
        const Point2dF p{4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{2, 0};
        const Point2dF p{8.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }


    {
        const HexCoordAxialPointy ph{-1, 0};
        const Point2dF p{-4.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
    {
        const HexCoordAxialPointy ph{-2, 0};
        const Point2dF p{-8.f, 0.f};
        const Point2dF pr = cs.to_draw_space(ph);
        EXPECT_EQ(p, pr);
    }
}
