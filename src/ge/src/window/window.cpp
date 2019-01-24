#include "window.h"

#if defined VK_USE_PLATFORM_WIN32_KHR
#include "win32/window_win32.h"
#elif defined VK_USE_PLATFORM_XCB_KHR
#include "linux/window_xcb.h"
#endif

namespace ge::impl
{

    Window::Window() = default;
    Window::~Window() = default;

    std::unique_ptr<Window> Window::create(uint16_t width, uint16_t height)
    {
#if defined VK_USE_PLATFORM_WIN32_KHR
        return std::make_unique<WindowWin32>();
#elif defined VK_USE_PLATFORM_XCB_KHR
        return std::make_unique<WindowXCB>(width, height);
#else
        throw std::runtime_error("There is no available window system");
#endif        
    }

}
