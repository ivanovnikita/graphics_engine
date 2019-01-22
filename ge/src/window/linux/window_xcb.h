#pragma once

#include "window/window.h"

#include <xcb/xcb.h>

namespace ge::impl
{

    class WindowXCB final : public Window
    {
    public:
        WindowXCB(uint16_t width, uint16_t height);
        ~WindowXCB() override;

        vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) override;
        vk::Extent2D extent() const override;

        void start_display() override;
        void process_events() override;
        bool closed() const override;

    private:
        xcb_connection_t* connection_;
        xcb_window_t handle_;
        uint16_t width_;
        uint16_t height_;
        xcb_intern_atom_reply_t* delete_reply_;
        bool closed_;
    };

}
