#pragma once

#include "ge/window/window_size.h"

#include <cstdint>
#include <variant>

namespace ge
{
    struct WindowEventClose final
    {
    };

    struct WindowEventResize final
    {
        Size new_size;
    };

    using WindowEvent = std::variant
    <
        WindowEventClose
        , WindowEventResize
    >;
}
