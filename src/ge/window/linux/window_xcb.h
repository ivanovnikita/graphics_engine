#pragma once

#include "ge/window/window.h"

#include <xcb/xcb.h>

#include <xcb/xcb_errors.h>
#include <xcb/xcb_keysyms.h>

#include <vulkan/vulkan.hpp>

namespace ge
{
    class WindowXCB final : public Window
    {
    public:
        WindowXCB
        (
            const WindowSize&
            , const std::array<uint8_t, 4> background_color
        );
        ~WindowXCB() override;

        vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) override;

        void start_display() override;
        std::vector<WindowEvent> grab_events() override;
        void resize(const Size&) override;
        void set_window_title(std::string_view) override;

    private:
        template <typename T>
        void init_window_size(const T&);

        template <typename T>
        void init_window_size_constraints(const T&);

        void init_key_mapping(const xcb_setup_t&);

        xcb_connection_t* connection_;
        xcb_errors_context_t* errors_ctx_;
        xcb_key_symbols_t* key_syms_;
        xcb_window_t handle_;
        Size current_size_;
        xcb_intern_atom_reply_t* delete_reply_;
    };
}
