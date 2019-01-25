#include "ge/render/render.h"
#include "ge/window/window.h"

#include <gtest/gtest.h>

TEST(Render, create)
{
    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    auto window = ge::Window::create(width, height);

    ASSERT_NO_THROW
    (
        ge::Render render
        (
            [&window] (const vk::Instance& instance)
            {
                return window->create_surface(instance);
            }
            , width
            , height
        )
    );
}
