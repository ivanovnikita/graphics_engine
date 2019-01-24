#include "render.h"

#include <thread>

int main()
{
    ge::Render render;

    render.draw_frame();
    while (not render.stopped())
    {
        render.process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
