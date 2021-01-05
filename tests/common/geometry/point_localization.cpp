#include "ge/geometry/point_localization.hpp"

#include <gtest/gtest.h>

TEST(point_localization, left)
{
    using namespace ge;

    const Point2dF a{0.f, 0.5f};
    const Point2dF b{0.25f, 1.f};

    const Point2dF p{0.f, 1.f};

    const LineSide result = line_side({a, b}, p);
    EXPECT_EQ(LineSide::Left, result);

    EXPECT_TRUE(is_left_side({a, b}, p));
}

TEST(point_localization, right)
{
    using namespace ge;

    const Point2dF a{0.25f, 1.f};
    const Point2dF b{0.f, 0.5f};

    const Point2dF p{0.f, 1.f};

    const LineSide result = line_side({a, b}, p);
    EXPECT_EQ(LineSide::Right, result);

    EXPECT_FALSE(is_left_side({a, b}, p));
}

TEST(point_localization, collinear)
{
    using namespace ge;

    const Point2dF a{0.25f, 0.f};
    const Point2dF b{0.75f, 0.f};

    const Point2dF p{0.5f, 0.f};

    const LineSide result = line_side({a, b}, p);
    EXPECT_EQ(LineSide::Collinear, result);

    EXPECT_FALSE(is_left_side({a, b}, p));
}
