#include "ge/graphics_engine.h"

#include <thread>

int main()
{
    ge::GraphicsEngine engine;

    engine.draw_frame();
    while (not engine.stopped())
    {
        engine.process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
