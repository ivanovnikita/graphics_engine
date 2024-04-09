module;

#include <gtest/gtest.h>

export module tests.hex_coord_rotate;

import cs_hex_flat;
import cs_hex_pointy;
import hex_coord_rotate;

module : private;

// TODO: use typed tests

TEST(hex_coord_rotate, flat_cw)
{
    using namespace ge;

    {
        const HexCoordAxialFlat p{0, 0};
        const HexCoordAxialFlat e{0, 0};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{2, -4};
        const HexCoordAxialFlat e{4, -2};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{4, -2};
        const HexCoordAxialFlat e{2, 2};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{2, 2};
        const HexCoordAxialFlat e{-2, 4};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-2, 4};
        const HexCoordAxialFlat e{-4, 2};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-4, 2};
        const HexCoordAxialFlat e{-2, -2};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-2, -2};
        const HexCoordAxialFlat e{2, -4};
        const HexCoordAxialFlat r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }
}

TEST(hex_coord_rotate, flat_ccw)
{
    using namespace ge;

    {
        const HexCoordAxialFlat p{0, 0};
        const HexCoordAxialFlat e{0, 0};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{2, -4};
        const HexCoordAxialFlat e{-2, -2};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-2, -2};
        const HexCoordAxialFlat e{-4, 2};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-4, 2};
        const HexCoordAxialFlat e{-2, 4};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{-2, 4};
        const HexCoordAxialFlat e{2, 2};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{2, 2};
        const HexCoordAxialFlat e{4, -2};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialFlat p{4, -2};
        const HexCoordAxialFlat e{2, -4};
        const HexCoordAxialFlat r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }
}

TEST(hex_coord_rotate, pointy_cw)
{
    using namespace ge;

    {
        const HexCoordAxialPointy p{0, 0};
        const HexCoordAxialPointy e{0, 0};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{2, -4};
        const HexCoordAxialPointy e{4, -2};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{4, -2};
        const HexCoordAxialPointy e{2, 2};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{2, 2};
        const HexCoordAxialPointy e{-2, 4};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-2, 4};
        const HexCoordAxialPointy e{-4, 2};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-4, 2};
        const HexCoordAxialPointy e{-2, -2};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-2, -2};
        const HexCoordAxialPointy e{2, -4};
        const HexCoordAxialPointy r = rotate_cw(p);
        EXPECT_EQ(e, r);
    }
}

TEST(hex_coord_rotate, pointy_ccw)
{
    using namespace ge;

    {
        const HexCoordAxialPointy p{0, 0};
        const HexCoordAxialPointy e{0, 0};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{2, -4};
        const HexCoordAxialPointy e{-2, -2};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-2, -2};
        const HexCoordAxialPointy e{-4, 2};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-4, 2};
        const HexCoordAxialPointy e{-2, 4};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{-2, 4};
        const HexCoordAxialPointy e{2, 2};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{2, 2};
        const HexCoordAxialPointy e{4, -2};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }

    {
        const HexCoordAxialPointy p{4, -2};
        const HexCoordAxialPointy e{2, -4};
        const HexCoordAxialPointy r = rotate_ccw(p);
        EXPECT_EQ(e, r);
    }
}
