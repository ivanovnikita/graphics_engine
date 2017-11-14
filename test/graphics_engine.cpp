#include <gtest/gtest.h>

#include "ge/graphics_engine.h"

#include <iostream>

TEST(GraphicsEngine, create)
{
    ASSERT_NO_THROW
    ({
        try
        {
            ge::GraphicsEngine ge;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    });
}
