#pragma once

#include <xcb/xcb.h>

#include <functional>
#include <array>
#include <variant>

#include <cstdint>

namespace ge
{
    struct XcbSurface final
    {
        std::reference_wrapper<xcb_connection_t> connection;
        xcb_window_t window;
    };

    using Surface = std::variant
    <
        XcbSurface
    >;

    struct SurfaceParams final
    {
        Surface surface;
        uint32_t width;
        uint32_t height;
        std::array<uint8_t, 4> background_color;
    };
}
