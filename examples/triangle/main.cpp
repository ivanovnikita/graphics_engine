#include "ge/render/render.h"
#include "ge/window/window.h"

#include <thread>

int main()
{
    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    auto window = ge::Window::create(width, height);

    ge::Render render
    (
        [&window] (const vk::Instance& instance)
        {
            return window->create_surface(instance);
        }
        , width
        , height
    );

    window->start_display();

    render.draw_frame();
    while (not window->closed())
    {
        window->process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
