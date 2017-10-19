#include "graphics_engine.h"

#include <iostream>

int main()
{
    try
    {
        ge::GraphicsEngine graphics_engine;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
