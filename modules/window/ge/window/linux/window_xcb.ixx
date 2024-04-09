module;

#include <xcb/xcb.h>

#include <xcb/xcb_errors.h>
#include <xcb/xcb_keysyms.h>

#include <memory>
#include <vector>

export module window_xcb;

export import window_size;
export import logger;
export import window_i;

namespace ge
{
    export class WindowXCB final : public WindowI
    {
    public:
        WindowXCB
        (
            const WindowSize&,
            const std::array<uint8_t, 4> background_color,
            const Logger& logger
        );
        ~WindowXCB() override;

        void start_display() override;
        std::vector<WindowEvent> grab_events() override;
        void resize(const Size&) override;
        void set_window_title(std::string_view) override;

        xcb_connection_t& get_connection() const noexcept;
        xcb_window_t get_window() const noexcept;

    private:
        template <typename T>
        void init_window_size(const T&) noexcept;

        template <typename T>
        void init_window_size_constraints(const T&) noexcept;

        void init_key_mapping(const xcb_setup_t&) noexcept;

        std::reference_wrapper<const Logger> logger_;
        std::unique_ptr<xcb_connection_t, void(*)(xcb_connection_t*)> connection_;
        std::unique_ptr<xcb_errors_context_t, void(*)(xcb_errors_context_t*)> errors_ctx_;
        std::unique_ptr<xcb_key_symbols_t, void(*)(xcb_key_symbols_t*)> key_syms_;
        xcb_window_t handle_;
        Size current_size_;
        std::unique_ptr<xcb_intern_atom_reply_t, void(*)(void*)> delete_reply_;
    };
}
