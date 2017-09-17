#pragma once

#include "window.h"

#include <xcb/xcb.h>

namespace ge::impl
{

    class WindowXCB final : public Window
    {
    public:
        WindowXCB();
        ~WindowXCB();

        std::unique_ptr<vk::UniqueSurfaceKHR> create_surface(const vk::UniqueInstance& instance) final;
    private:
        xcb_connection_t* connection_;
        xcb_window_t handle_;
    };

} // namespace ge::impl
