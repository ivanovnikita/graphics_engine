#include <gtest/gtest.h>

#include "ge/graphics_engine.h"

TEST(GraphicsEngine, create)
{
    ASSERT_NO_THROW({ge::GraphicsEngine ge;});
}
