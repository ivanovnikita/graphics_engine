#pragma once

#include <cstdint>
#include <variant>

namespace ge
{
    struct WindowEventClose final
    {
    };

    struct WindowEventResize final
    {
        uint16_t new_width;
        uint16_t new_height;
    };

    using WindowEvent = std::variant
    <
        WindowEventClose
        , WindowEventResize
    >;
}
