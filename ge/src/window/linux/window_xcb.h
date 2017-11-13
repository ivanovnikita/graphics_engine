#pragma once

#include "window.h"

#include <xcb/xcb.h>

namespace ge::impl
{

    class WindowXCB final : public Window
    {
    public:
        WindowXCB(uint16_t width, uint16_t height);
        ~WindowXCB();

        std::unique_ptr<vk::UniqueSurfaceKHR> create_surface(const vk::UniqueInstance& instance) final;
        std::pair<uint16_t, uint16_t> extent() const final;
    private:
        xcb_connection_t* connection_;
        xcb_window_t handle_;
        uint16_t width_;
        uint16_t height_;
    };

} // namespace ge::impl
