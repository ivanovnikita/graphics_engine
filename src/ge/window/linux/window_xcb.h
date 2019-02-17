#pragma once

#include "ge/window/window.h"

#include <xcb/xcb.h>

#include <vulkan/vulkan.hpp>

namespace ge
{
    class WindowXCB final : public Window
    {
    public:
        WindowXCB(const WindowSize&);
        ~WindowXCB() override;

        vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) override;

        void start_display() override;
        std::vector<WindowEvent> grab_events() override;
        void resize(const Size&) override;

    private:
        template <typename T>
        void init_window_size(const T&);

        template <typename T>
        void init_window_size_constraints(const T&);

        xcb_connection_t* connection_;
        xcb_window_t handle_;
        Size current_size_;
        xcb_intern_atom_reply_t* delete_reply_;
    };
}
