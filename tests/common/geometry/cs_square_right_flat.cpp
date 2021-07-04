#include "ge/geometry/cs_square_right_flat.hpp"

#include <gtest/gtest.h>

TEST(cs_square_right_flat, draw_space_to_axial)
{
    using namespace ge;

    const CsSquareRightFlat cs{4.f, 2.f, 1.f};

    {
        const Point2dF p{0.f, 0.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{0.5f, 0.5f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.f, 0.9f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{1.99f, 0.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.f, 0.f};
        const SquareRightCoordAxial expected{1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.01f, 0.f};
        const SquareRightCoordAxial expected{1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{1.5f, -1.f};
        const SquareRightCoordAxial expected{1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{1.49f, -1.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{1.51f, -1.f};
        const SquareRightCoordAxial expected{1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{2.5f, 1.f};
        const SquareRightCoordAxial expected{1, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.49f, 1.f};
        const SquareRightCoordAxial expected{0, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.51f, 1.f};
        const SquareRightCoordAxial expected{1, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{-1.5f, 1.f};
        const SquareRightCoordAxial expected{0, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-1.49f, 1.f};
        const SquareRightCoordAxial expected{0, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-1.51f, 1.f};
        const SquareRightCoordAxial expected{-1, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{-1.99f, 0.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.f, 0.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.01f, 0.f};
        const SquareRightCoordAxial expected{-1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{-2.5f, -1.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.49f, -1.f};
        const SquareRightCoordAxial expected{0, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.51f, -1.f};
        const SquareRightCoordAxial expected{-1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{1.f, 2.f};
        const SquareRightCoordAxial expected{0, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{5.f, 2.f};
        const SquareRightCoordAxial expected{1, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{4.f, 0.f};
        const SquareRightCoordAxial expected{1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{3.f, -2.f};
        const SquareRightCoordAxial expected{1, -1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-1.f, -2.f};
        const SquareRightCoordAxial expected{0, -1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-5.f, -2.f};
        const SquareRightCoordAxial expected{-1, -1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-4.f, 0.f};
        const SquareRightCoordAxial expected{-1, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-3.f, 2.f};
        const SquareRightCoordAxial expected{-1, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }

    {
        const Point2dF p{2.f, 4.f};
        const SquareRightCoordAxial expected{0, 2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{6.f, 4.f};
        const SquareRightCoordAxial expected{1, 2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{10.f, 4.f};
        const SquareRightCoordAxial expected{2, 2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{9.f, 2.f};
        const SquareRightCoordAxial expected{2, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{8.f, 0.f};
        const SquareRightCoordAxial expected{2, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{7.f, -2.f};
        const SquareRightCoordAxial expected{2, -1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{6.f, -4.f};
        const SquareRightCoordAxial expected{2, -2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{2.f, -4.f};
        const SquareRightCoordAxial expected{1, -2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.f, -4.f};
        const SquareRightCoordAxial expected{0, -2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-6.f, -4.f};
        const SquareRightCoordAxial expected{-1, -2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-10.f, -4.f};
        const SquareRightCoordAxial expected{-2, -2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-9.f, -2.f};
        const SquareRightCoordAxial expected{-2, -1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-8.f, 0.f};
        const SquareRightCoordAxial expected{-2, 0};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-7.f, 2.f};
        const SquareRightCoordAxial expected{-2, 1};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-6.f, 4.f};
        const SquareRightCoordAxial expected{-2, 2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
    {
        const Point2dF p{-2.f, 4.f};
        const SquareRightCoordAxial expected{-1, 2};
        const SquareRightCoordAxial result = cs.to_axial(p);
        EXPECT_EQ(expected, result);
    }
}

TEST(cs_square_right_flat, axial_to_draw_space)
{
    using namespace ge;

    const CsSquareRightFlat cs{4.f, 2.f, 1.f};

    {
        const SquareRightCoordAxial p{0, 0};
        const Point2dF expected{0.f, 0.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }

    {
        const SquareRightCoordAxial p{1, 0};
        const Point2dF expected{4.f, 0.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{0, 1};
        const Point2dF expected{1.f, 2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{1, 1};
        const Point2dF expected{5.f, 2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-1, 0};
        const Point2dF expected{-4.f, 0.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{0, -1};
        const Point2dF expected{-1.f, -2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-1, -1};
        const Point2dF expected{-5.f, -2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{1, -1};
        const Point2dF expected{3.f, -2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-1, 1};
        const Point2dF expected{-3.f, 2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }

    {
        const SquareRightCoordAxial p{2, 2};
        const Point2dF expected{10.f, 4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{2, 1};
        const Point2dF expected{9.f, 2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{2, 0};
        const Point2dF expected{8.f, 0.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{2, -1};
        const Point2dF expected{7.f, -2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{2, -2};
        const Point2dF expected{6.f, -4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{1, -2};
        const Point2dF expected{2.f, -4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{0, -2};
        const Point2dF expected{-2.f, -4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-1, -2};
        const Point2dF expected{-6.f, -4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-2, -2};
        const Point2dF expected{-10.f, -4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-2, -1};
        const Point2dF expected{-9.f, -2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-2, 0};
        const Point2dF expected{-8.f, 0.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-2, 1};
        const Point2dF expected{-7.f, 2.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-2, 2};
        const Point2dF expected{-6.f, 4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{-1, 2};
        const Point2dF expected{-2.f, 4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{0, 2};
        const Point2dF expected{2.f, 4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
    {
        const SquareRightCoordAxial p{1, 2};
        const Point2dF expected{6.f, 4.f};
        const Point2dF result = cs.to_draw_space(p);
        EXPECT_EQ(expected, result);
    }
}
