module;

#include <xcb/xcb.h>

#include <functional>
#include <array>
#include <variant>

#include <cstdint>

export module vulkan_common.surface_params;

namespace ge
{
    export struct XcbSurface final
    {
        std::reference_wrapper<xcb_connection_t> connection;
        xcb_window_t window;
    };

    export using Surface = std::variant
    <
        XcbSurface
    >;

    export struct SurfaceParams final
    {
        Surface surface;
        uint32_t width;
        uint32_t height;
        std::array<uint8_t, 4> background_color;
    };
}
