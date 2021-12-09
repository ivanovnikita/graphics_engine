#include "ge/window/window.h"

#if defined VK_USE_PLATFORM_WIN32_KHR
#include "ge/window/win32/window_win32.h"
#elif defined VK_USE_PLATFORM_XCB_KHR
#include "ge/window/linux/window_xcb.h"
#endif

namespace ge
{
    Window::Window() = default;
    Window::~Window() = default;

    std::unique_ptr<Window> Window::create
    (
        const WindowSize& size,
        const std::array<uint8_t, 4> background_color,
        const Logger& logger
    )
    {
#if defined VK_USE_PLATFORM_WIN32_KHR
        return std::make_unique<WindowWin32>();
#elif defined VK_USE_PLATFORM_XCB_KHR
        return std::make_unique<WindowXCB>(size, background_color, logger);
#else
        throw std::runtime_error("There is no available window system");
#endif        
    }
}
