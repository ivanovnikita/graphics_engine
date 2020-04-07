#pragma once

#include "ge/window/window_size.h"

#include <cstdint>
#include <variant>

namespace ge
{
    struct WindowExposed final
    {
    };

    struct WindowEventClose final
    {
    };

    struct WindowEventResize final
    {
        Size new_size;
    };

    struct WheelEvent final
    {
        enum class Direction
        {
            UP
            , DOWN
        };

        Direction direction;
    };

    using WindowEvent = std::variant
    <
        WindowExposed
        , WindowEventClose
        , WindowEventResize
        , WheelEvent
    >;
}
