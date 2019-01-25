#include "render/render.h"

#include <gtest/gtest.h>

TEST(Render, create)
{
    ASSERT_NO_THROW(ge::Render render);
}
