#include "ge/graphics_engine.h"

#include <thread>

int main()
{
    ge::GraphicsEngine engine;

    while (not engine.stopped())
    {
        engine.draw_frame();
        engine.process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
