#include "ge/window/linux/window_xcb.h"
#include "ge/common/overloaded.hpp"

#include <xcb/xcb_icccm.h>
#include <xcb/xcb_aux.h>
#include <xcb/xinput.h>

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>

#include <iostream>
#include <sstream>

namespace ge
{
    namespace
    {
        enum WindowPropertyFormat
        {
            BIT_8 = 8,
            BIT_16 = 16,
            BIT_32 = 32
        };

        enum InternAtom
        {
            CREATE_IF_NOT_EXISTS = 0,
            ONLY_IF_EXISTS = 1
        };

        std::unique_ptr<xcb_intern_atom_reply_t, void(*)(void*)> subscribe_to_close_event
        (
            xcb_connection_t& connection,
            const xcb_window_t& handle
        ) noexcept
        {
            constexpr std::string_view name_protocols("WM_PROTOCOLS");
            const xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom
            (
                &connection,
                InternAtom::ONLY_IF_EXISTS,
                name_protocols.length(),
                name_protocols.data()
            );
            const std::unique_ptr<xcb_intern_atom_reply_t, void(*)(void*)> protocols_reply
            {
                xcb_intern_atom_reply
                (
                    &connection,
                    protocols_cookie,
                    nullptr
                ),
                &free
            };
            assert(protocols_reply != nullptr);
            constexpr std::string_view delete_window("WM_DELETE_WINDOW");
            const xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom
            (
                &connection,
                InternAtom::CREATE_IF_NOT_EXISTS,
                delete_window.length(),
                delete_window.data()
            );
            std::unique_ptr<xcb_intern_atom_reply_t, void(*)(void*)> delete_reply
            {
                xcb_intern_atom_reply
                (
                    &connection,
                    delete_cookie,
                    nullptr
                ),
                &free
            };
            assert(delete_reply != nullptr);
            xcb_change_property
            (
                &connection,
                XCB_PROP_MODE_REPLACE,
                handle,
                protocols_reply->atom,
                4,
                32,
                1,
                &delete_reply->atom
            );

            return delete_reply;
        }

        void set_min_max_sizes
        (
            xcb_connection_t& connection,
            const xcb_window_t& handle,
            const Size& current_size,
            const std::optional<Size>& min_size,
            const std::optional<Size>& max_size
        ) noexcept
        {
            assert(min_size.has_value() or max_size.has_value());

            xcb_size_hints_t hints;
            std::memset(&hints, 0, sizeof(hints));

            xcb_icccm_size_hints_set_size(&hints, true, current_size.width, current_size.height);

            if (min_size.has_value())
            {
                xcb_icccm_size_hints_set_min_size(&hints, min_size->width, min_size->height);
            }
            if (max_size.has_value())
            {
                xcb_icccm_size_hints_set_max_size(&hints, max_size->width, max_size->height);
            }

            xcb_icccm_set_wm_normal_hints(&connection, handle, &hints);
        }

        ModifiersState parse_modifiers(uint16_t state) noexcept
        {
            const auto extract_state_to = [&state] (bool& field, uint8_t& counter)
            {
                bool result = false;
                if (state & 1)
                {
                    field = true;
                    ++counter;
                }
                state >>= static_cast<uint16_t>(1);
                return result;
            };

            ModifiersState result;

            extract_state_to(result.shift, result.count);
            extract_state_to(result.caps_lock, result.count);
            extract_state_to(result.ctrl, result.count);
            extract_state_to(result.mod_1, result.count);
            extract_state_to(result.mod_2, result.count);
            extract_state_to(result.mod_3, result.count);
            extract_state_to(result.mod_4, result.count);
            extract_state_to(result.mod_5, result.count);
            extract_state_to(result.mouse_left, result.count);
            extract_state_to(result.mouse_middle, result.count);
            extract_state_to(result.mouse_right, result.count);
            extract_state_to(result.scroll_up, result.count);
            extract_state_to(result.scroll_down, result.count);

            return result;
        }

        std::string error_event_to_string
        (
            xcb_errors_context_t& errors_ctx,
            const xcb_generic_error_t& e
        )
        {
            std::string result{"X error: request="};

            const char* major = xcb_errors_get_name_for_major_code(&errors_ctx, e.major_code);
            const char* minor = xcb_errors_get_name_for_minor_code(&errors_ctx, e.major_code, e.minor_code);
            const char* extension = nullptr;
            const char* error = xcb_errors_get_name_for_error(&errors_ctx, e.error_code, &extension);

            assert(major != nullptr);
            result += major;

            if (minor != nullptr)
            {
                result += "-";
                result += minor;
            }

            result += " (major ";
            result += static_cast<char>(e.major_code);
            result += ", minor ";
            result += static_cast<char>(e.minor_code);
            result += "), error=";

            if (extension != nullptr)
            {
                result += extension;
                result += "-";
            }

            assert(error != nullptr);
            result += error;

            return result;
        }

        template <ButtonEvent button_event>
        std::optional<WindowEvent> handle_mouse_button_event(const xcb_button_press_event_t& event) noexcept
        {
            const glm::vec2 pos{event.event_x, event.event_y};
            const ModifiersState modifiers{parse_modifiers(event.state)};
            const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

            switch (event.detail)
            {
            case XCB_BUTTON_INDEX_1:
            {
                return MouseButtonEvent<MouseButton, button_event>{MouseButton::LEFT, pos, modifiers, timestamp};
            }
            case XCB_BUTTON_INDEX_2:
            {
                return MouseButtonEvent<MouseButton, button_event>{MouseButton::MIDDLE, pos, modifiers, timestamp};
            }
            case XCB_BUTTON_INDEX_3:
            {
                return MouseButtonEvent<MouseButton, button_event>{MouseButton::RIGHT, pos, modifiers, timestamp};
            }
            }

            if constexpr (button_event == ButtonEvent::PRESS)
            {
                switch (event.detail)
                {
                    case XCB_BUTTON_INDEX_4:
                    {
                        return WheelEvent{ScrollButton::UP, pos, modifiers, timestamp};
                    }
                    case XCB_BUTTON_INDEX_5:
                    {
                        return WheelEvent{ScrollButton::DOWN, pos, modifiers, timestamp};
                    }
                }
            }

            return std::nullopt;
        }

        WindowEvent handle_mouse_motion_event(const xcb_motion_notify_event_t& event) noexcept
        {
            const glm::vec2 pos{event.event_x, event.event_y};
            const ModifiersState modifiers{parse_modifiers(event.state)};
            const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

            return MouseMoveEvent{pos, modifiers, timestamp};
        }

        template <CrossEvent cross_event>
        WindowEvent handle_cross_window_border_event(const xcb_enter_notify_event_t& event) noexcept
        {
            const glm::vec2 pos{event.event_x, event.event_y};
            const ModifiersState modifiers{parse_modifiers(event.state)};
            const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

            return MouseCrossWindowBorderEvent<cross_event>
            {
                pos,
                modifiers,
                timestamp
            };
        }

        std::variant
        <
            ServiceKey,
            char,
            KeypadServiceKey,
            KeypadLatinKey,
            uint32_t
        > match_key(const xcb_keysym_t sym) noexcept
        {
            switch (sym)
            {
            case XK_Return: return ServiceKey::Enter;
            case XK_Tab: return ServiceKey::Tab;

            case XK_BackSpace: return ServiceKey::BackSpace;
            case XK_Linefeed: return ServiceKey::Linefeed;
            case XK_Clear: return ServiceKey::Clear;
            case XK_Pause: return ServiceKey::Pause;
            case XK_Scroll_Lock: return ServiceKey::ScrollLock;
            case XK_Sys_Req: return ServiceKey::SysReq;
            case XK_Escape: return ServiceKey::Escape;
            case XK_Delete: return ServiceKey::Delete;

            case XK_Home: return ServiceKey::Home;
            case XK_Left: return ServiceKey::Left;
            case XK_Up: return ServiceKey::Up;
            case XK_Right: return ServiceKey::Right;
            case XK_Down: return ServiceKey::Down;
            case XK_Page_Up: return ServiceKey::PageUp;
            case XK_Page_Down: return ServiceKey::PageDown;
            case XK_End: return ServiceKey::End;
            case XK_Begin: return ServiceKey::Begin;

            case XK_Select: return ServiceKey::Select;
            case XK_Print: return ServiceKey::Print;
            case XK_Execute: return ServiceKey::Execute;
            case XK_Insert: return ServiceKey::Insert;
            case XK_Undo: return ServiceKey::Undo;
            case XK_Redo: return ServiceKey::Redo;
            case XK_Menu: return ServiceKey::Menu;
            case XK_Find: return ServiceKey::Find;
            case XK_Cancel: return ServiceKey::Cancel;
            case XK_Help: return ServiceKey::Help;
            case XK_Break: return ServiceKey::Break;
            case XK_Mode_switch: return ServiceKey::ModeSwitch;
            case XK_Num_Lock: return ServiceKey::NumLock;

            case XK_F1: return ServiceKey::F1;
            case XK_F2: return ServiceKey::F2;
            case XK_F3: return ServiceKey::F3;
            case XK_F4: return ServiceKey::F4;
            case XK_F5: return ServiceKey::F5;
            case XK_F6: return ServiceKey::F6;
            case XK_F7: return ServiceKey::F7;
            case XK_F8: return ServiceKey::F8;
            case XK_F9: return ServiceKey::F9;
            case XK_F10: return ServiceKey::F10;
            case XK_F11: return ServiceKey::F11;
            case XK_F12: return ServiceKey::F12;

            case XK_Shift_L: return ServiceKey::ShiftL;
            case XK_Shift_R: return ServiceKey::ShiftR;
            case XK_Control_L: return ServiceKey::ControlL;
            case XK_Control_R: return ServiceKey::ControlR;
            case XK_Alt_L: return ServiceKey::AltL;
            case XK_Alt_R: return ServiceKey::AltR;
            case XK_Caps_Lock: return ServiceKey::CapsLock;
            case XK_Shift_Lock: return ServiceKey::ShiftLock;
            case XK_Meta_L: return ServiceKey::MetaL;
            case XK_Meta_R: return ServiceKey::MetaR;
            case XK_Super_L: return ServiceKey::SuperL;
            case XK_Super_R: return ServiceKey::SuperR;
            case XK_Hyper_L: return ServiceKey::HyperL;
            case XK_Hyper_R: return ServiceKey::HyperR;

            case XK_space:
            case XK_exclam:
            case XK_quotedbl:
            case XK_numbersign:
            case XK_dollar:
            case XK_percent:
            case XK_ampersand:
            case XK_apostrophe:
            case XK_parenleft:
            case XK_parenright:
            case XK_asterisk:
            case XK_plus:
            case XK_comma:
            case XK_minus:
            case XK_period:
            case XK_slash:
            case XK_0:
            case XK_1:
            case XK_2:
            case XK_3:
            case XK_4:
            case XK_5:
            case XK_6:
            case XK_7:
            case XK_8:
            case XK_9:
            case XK_colon:
            case XK_semicolon:
            case XK_less:
            case XK_equal:
            case XK_greater:
            case XK_question:
            case XK_at:
            case XK_A:
            case XK_B:
            case XK_C:
            case XK_D:
            case XK_E:
            case XK_F:
            case XK_G:
            case XK_H:
            case XK_I:
            case XK_J:
            case XK_K:
            case XK_L:
            case XK_M:
            case XK_N:
            case XK_O:
            case XK_P:
            case XK_Q:
            case XK_R:
            case XK_S:
            case XK_T:
            case XK_U:
            case XK_V:
            case XK_W:
            case XK_X:
            case XK_Y:
            case XK_Z:
            case XK_bracketleft:
            case XK_backslash:
            case XK_bracketright:
            case XK_asciicircum:
            case XK_underscore:
            case XK_quoteleft:
            case XK_a:
            case XK_b:
            case XK_c:
            case XK_d:
            case XK_e:
            case XK_f:
            case XK_g:
            case XK_h:
            case XK_i:
            case XK_j:
            case XK_k:
            case XK_l:
            case XK_m:
            case XK_n:
            case XK_o:
            case XK_p:
            case XK_q:
            case XK_r:
            case XK_s:
            case XK_t:
            case XK_u:
            case XK_v:
            case XK_w:
            case XK_x:
            case XK_y:
            case XK_z:
            case XK_braceleft:
            case XK_bar:
            case XK_braceright:
            case XK_asciitilde:
            {
                return *reinterpret_cast<const char*>(&sym);
            }

            case XK_KP_Space: return KeypadServiceKey::Space;
            case XK_KP_Tab: return KeypadServiceKey::Tab;
            case XK_KP_Enter: return KeypadServiceKey::Enter;
            case XK_KP_F1: return KeypadServiceKey::F1;
            case XK_KP_F2: return KeypadServiceKey::F2;
            case XK_KP_F3: return KeypadServiceKey::F3;
            case XK_KP_F4: return KeypadServiceKey::F4;
            case XK_KP_Home: return KeypadServiceKey::Home;
            case XK_KP_Left: return KeypadServiceKey::Left;
            case XK_KP_Up: return KeypadServiceKey::Up;
            case XK_KP_Right: return KeypadServiceKey::Right;
            case XK_KP_Down: return KeypadServiceKey::Down;
            case XK_KP_Page_Up: return KeypadServiceKey::PageUp;
            case XK_KP_Page_Down: return KeypadServiceKey::PageDown;
            case XK_KP_End: return KeypadServiceKey::End;
            case XK_KP_Begin: return KeypadServiceKey::Begin;
            case XK_KP_Insert: return KeypadServiceKey::Insert;
            case XK_KP_Delete: return KeypadServiceKey::Delete;
            case XK_KP_Decimal: return KeypadServiceKey::Decimal;

            case XK_KP_Equal: return KeypadLatinKey::Equal;
            case XK_KP_Multiply: return KeypadLatinKey::Multiply;
            case XK_KP_Add: return KeypadLatinKey::Add;
            case XK_KP_Separator: return KeypadLatinKey::Separator;
            case XK_KP_Subtract: return KeypadLatinKey::Subtract;
            case XK_KP_Divide: return KeypadLatinKey::Divide;
            case XK_KP_0: return KeypadLatinKey::KP_0;
            case XK_KP_1: return KeypadLatinKey::KP_1;
            case XK_KP_2: return KeypadLatinKey::KP_2;
            case XK_KP_3: return KeypadLatinKey::KP_3;
            case XK_KP_4: return KeypadLatinKey::KP_4;
            case XK_KP_5: return KeypadLatinKey::KP_5;
            case XK_KP_6: return KeypadLatinKey::KP_6;
            case XK_KP_7: return KeypadLatinKey::KP_7;
            case XK_KP_8: return KeypadLatinKey::KP_8;
            case XK_KP_9: return KeypadLatinKey::KP_9;
            }

            return sym;
        }

        [[ maybe_unused ]] std::string_view get_name(const ServiceKey k) noexcept
        {
            switch (k)
            {
            case ServiceKey::Enter: return "Enter";
            case ServiceKey::Tab: return "Tab";

            case ServiceKey::BackSpace: return "Backspace";
            case ServiceKey::Linefeed: return "Linefeed";
            case ServiceKey::Clear: return "Clear";
            case ServiceKey::Pause: return "Pause";
            case ServiceKey::ScrollLock: return "ScrollLock";
            case ServiceKey::SysReq: return "SysReq";
            case ServiceKey::Escape: return "Escape";
            case ServiceKey::Delete: return "Delete";

            case ServiceKey::Home: return "Home";
            case ServiceKey::Left: return "Left";
            case ServiceKey::Up: return "Up";
            case ServiceKey::Right: return "Right";
            case ServiceKey::Down: return "Down";
            case ServiceKey::PageUp: return "PageUp";
            case ServiceKey::PageDown: return "PageDown";
            case ServiceKey::End: return "End";
            case ServiceKey::Begin: return "Begin";

            case ServiceKey::Select: return "Select";
            case ServiceKey::Print: return "Print";
            case ServiceKey::Execute: return "Execute";
            case ServiceKey::Insert: return "Insert";
            case ServiceKey::Undo: return "Undo";
            case ServiceKey::Redo: return "Redo";
            case ServiceKey::Menu: return "Menu";
            case ServiceKey::Find: return "Find";
            case ServiceKey::Cancel: return "Cancel";
            case ServiceKey::Help: return "Help";
            case ServiceKey::Break: return "Break";
            case ServiceKey::ModeSwitch: return "ModeSwitch";
            case ServiceKey::NumLock: return "NumLock";

            case ServiceKey::F1: return "F1";
            case ServiceKey::F2: return "F2";
            case ServiceKey::F3: return "F3";
            case ServiceKey::F4: return "F4";
            case ServiceKey::F5: return "F5";
            case ServiceKey::F6: return "F6";
            case ServiceKey::F7: return "F7";
            case ServiceKey::F8: return "F8";
            case ServiceKey::F9: return "F9";
            case ServiceKey::F10: return "F10";
            case ServiceKey::F11: return "F11";
            case ServiceKey::F12: return "F12";

            case ServiceKey::ShiftL: return "ShiftL";
            case ServiceKey::ShiftR: return "ShiftR";
            case ServiceKey::ControlL: return "ControlL";
            case ServiceKey::ControlR: return "ControlR";
            case ServiceKey::AltL: return "AltL";
            case ServiceKey::AltR: return "AltR";
            case ServiceKey::CapsLock: return "CapsLock";
            case ServiceKey::ShiftLock: return "ShiftLock";
            case ServiceKey::MetaL: return "MetaL";
            case ServiceKey::MetaR: return "MetaR";
            case ServiceKey::SuperL: return "SuperL";
            case ServiceKey::SuperR: return "SuperR";
            case ServiceKey::HyperL: return "HyperL";
            case ServiceKey::HyperR: return "HyperR";
            }

            __builtin_unreachable();
        }

        [[ maybe_unused ]] std::string_view get_name(const KeypadServiceKey k)
        {
            switch (k)
            {
            case KeypadServiceKey::Space: return "Space";
            case KeypadServiceKey::Tab: return "Tab";
            case KeypadServiceKey::Enter: return "Enter";
            case KeypadServiceKey::F1: return "F1";
            case KeypadServiceKey::F2: return "F2";
            case KeypadServiceKey::F3: return "F3";
            case KeypadServiceKey::F4: return "F4";
            case KeypadServiceKey::Home: return "Home";
            case KeypadServiceKey::Left: return "Left";
            case KeypadServiceKey::Up: return "Up";
            case KeypadServiceKey::Right: return "Right";
            case KeypadServiceKey::Down: return "Down";
            case KeypadServiceKey::PageUp: return "PageUp";
            case KeypadServiceKey::PageDown: return "PageDown";
            case KeypadServiceKey::End: return "End";
            case KeypadServiceKey::Begin: return "Begin";
            case KeypadServiceKey::Insert: return "Insert";
            case KeypadServiceKey::Delete: return "Delete";
            case KeypadServiceKey::Decimal: return "Decimal";
            }

            __builtin_unreachable();
        }

        char get_symbol(const KeypadLatinKey k)
        {
            switch (k)
            {
            case KeypadLatinKey::Equal: return '=';
            case KeypadLatinKey::Multiply: return '*';
            case KeypadLatinKey::Add: return '+';
            case KeypadLatinKey::Separator: return '.';
            case KeypadLatinKey::Subtract: return '-';
            case KeypadLatinKey::Divide: return '/';
            case KeypadLatinKey::KP_0: return '0';
            case KeypadLatinKey::KP_1: return '1';
            case KeypadLatinKey::KP_2: return '2';
            case KeypadLatinKey::KP_3: return '3';
            case KeypadLatinKey::KP_4: return '4';
            case KeypadLatinKey::KP_5: return '5';
            case KeypadLatinKey::KP_6: return '6';
            case KeypadLatinKey::KP_7: return '7';
            case KeypadLatinKey::KP_8: return '8';
            case KeypadLatinKey::KP_9: return '9';
            }

            __builtin_unreachable();
        }
    }

    template <>
    void WindowXCB::init_window_size<StaticSize>(const StaticSize& size) noexcept
    {
        current_size_ = size;
    }

    template <>
    void WindowXCB::init_window_size<DynamicSize>(const DynamicSize& size) noexcept
    {
        current_size_ = size.default_size;
    }

    template <>
    void WindowXCB::init_window_size_constraints(const StaticSize& size) noexcept
    {
        set_min_max_sizes(*connection_, handle_, size, size, size);
    }

    template <>
    void WindowXCB::init_window_size_constraints(const DynamicSize& size) noexcept
    {
        if (size.min_size.has_value() or size.max_size.has_value())
        {
            set_min_max_sizes(*connection_, handle_, size.default_size, size.min_size, size.max_size);
        }
    }

    void WindowXCB::init_key_mapping(const xcb_setup_t& /*setup*/) noexcept
    {
//        xcb_generic_error_t* error;

//        std::optional<uint8_t> keyboard_id;
//        {
//            const xcb_input_list_input_devices_cookie_t cookie = xcb_input_list_input_devices_unchecked
//            (
//                connection_
//            );
//            xcb_input_list_input_devices_reply_t* reply = xcb_input_list_input_devices_reply
//            (
//                connection_,
//                cookie,
//                &error
//            );
//            if (reply == nullptr)
//            {
//                std::string message = error_event_to_string(errors_ctx_, *error);
//                free(error);
//                throw std::runtime_error(std::move(message));
//            }

//            xcb_input_device_info_iterator_t device_info_it = xcb_input_list_input_devices_devices_iterator(reply);
//            while (device_info_it.rem != 0)
//            {
//                const xcb_input_device_info_t& device_info = *device_info_it.data;

//                std::cout << "device type: " << device_info.device_type << "\n"
//                    << "device id: " << int(device_info.device_id) << "\n"
//                    << "num class info: " << int(device_info.num_class_info) << "\n"
//                    << "device use: " << int(device_info.device_use) << "\n" <<std::endl;

//                xcb_input_device_use_t device_use = static_cast<xcb_input_device_use_t>(device_info.device_use);
//                if (device_use == XCB_INPUT_DEVICE_USE_IS_X_KEYBOARD)
//                {
//                    keyboard_id.emplace(device_info.device_id);
//                    break;
//                }

//                xcb_input_device_info_next(&device_info_it);
//            }

//            free(reply);
//        }

//        if (not keyboard_id.has_value())
//        {
//            std::cout << "keyboard id not found" << std::endl;
//            return;
//        }

        // TODO: try to use xcb_get_keyboard_mapping_cookie_t (without device id)

//        {

//            const xcb_input_get_device_key_mapping_cookie_t cookie = xcb_input_get_device_key_mapping
//            (
//                connection_,
//                *keyboard_id,
//                setup.min_keycode,
//                setup.max_keycode - setup.min_keycode + 1
//            );
//            xcb_input_get_device_key_mapping_reply_t* reply = xcb_input_get_device_key_mapping_reply
//            (
//                connection_,
//                cookie,
//                &error
//            );
//            if (reply == nullptr)
//            {
//                std::string message = error_event_to_string(errors_ctx_, *error);
//                free(error);
//                throw std::runtime_error(std::move(message));
//            }

//            xcb_keysym_t* keysyms = xcb_input_get_device_key_mapping_keysyms(reply);
//            assert(keysyms != nullptr);

//            const xcb_get_keyboard_mapping_cookie_t cookie = xcb_get_keyboard_mapping
//            (
//                connection_,
//                setup.min_keycode,
//                setup.max_keycode - setup.min_keycode + 1
//            );
//            xcb_get_keyboard_mapping_reply_t* reply = xcb_get_keyboard_mapping_reply
//            (
//                connection_,
//                cookie,
//                &error
//            );
//            if (reply == nullptr)
//            {
//                std::string message = error_event_to_string(errors_ctx_, *error);
//                free(error);
//                throw std::runtime_error(std::move(message));
//            }
//            xcb_keysym_t* keysyms = xcb_get_keyboard_mapping_keysyms(reply);
//            assert(keysyms != nullptr);

//            const size_t count = setup.max_keycode - setup.min_keycode + 1;
//            for (size_t i = 0; i < count; ++i)
//            {
//                std::string s{"1234"};
//                std::memcpy(s.data(), reinterpret_cast<char*>(&(keysyms[i])), sizeof(xcb_keysym_t));
//                std::cout << "keysym: " << s << "\n";
////                std::cout << "keysym: " << keysyms[i] << "\n";
//            }
//            std::cout << std::endl;
//        }

        key_syms_ = {xcb_key_symbols_alloc(connection_.get()), &xcb_key_symbols_free};
    }

    template <ButtonEvent button_event>
    WindowEvent handle_key_event
    (
        xcb_key_symbols_t& key_syms,
        xcb_key_press_event_t& event
    )
    {
        const ModifiersState modifiers{parse_modifiers(event.state)};
        const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

        // TODO: for keypad latin key use col == 1 if mod_2 if active
        int col = 0;
        if (modifiers.shift)
        {
            col = 1;
        }

        xcb_keysym_t sym = xcb_key_press_lookup_keysym(&key_syms, &event, col);

        std::variant
        <
            ServiceKey,
            char,
            KeypadServiceKey,
            KeypadLatinKey,
            uint32_t
        > parsed = match_key(sym);

        WindowEvent result = std::visit
        (
            overloaded
            {
                [&] (const ServiceKey k) -> WindowEvent
                {
                    return KeyEvent<KeyType::Service, button_event>
                    {
                        .key = k,
                        .modifiers = modifiers,
                        .timestamp = timestamp
                    };
                },
                [&] (const char c) -> WindowEvent
                {
                    return KeyEvent<KeyType::Latin, button_event>
                    {
                        .key = c,
                        .modifiers = modifiers,
                        .timestamp = timestamp
                    };
                },
                [&] (const KeypadServiceKey k) -> WindowEvent
                {
                    return KeyEvent<KeyType::KeypadService, button_event>
                    {
                        .key = k,
                        .modifiers = modifiers,
                        .timestamp = timestamp
                    };
                },
                [&] (const KeypadLatinKey k) -> WindowEvent
                {
                    return KeyEvent<KeyType::KeypadLatin, button_event>
                    {
                        .symbol = get_symbol(k),
                        .key = k,
                        .modifiers = modifiers,
                        .timestamp = timestamp
                    };
                },
                [&] (const uint32_t v) -> WindowEvent
                {
                    return KeyEvent<KeyType::Unknown, button_event>
                    {
                        .key = v,
                        .modifiers = modifiers,
                        .timestamp = timestamp
                    };
                }
            },
            parsed
        );

        return result;
    }

    WindowXCB::WindowXCB
    (
        const WindowSize& size,
        const std::array<uint8_t, 4> background_color,
        const Logger& logger
    )
        : logger_{logger}
        , connection_{nullptr, &xcb_disconnect}
        , errors_ctx_{nullptr, &xcb_errors_context_free}
        , key_syms_{nullptr, &xcb_key_symbols_free}
        , delete_reply_{nullptr, &free}
    {
        std::visit([this] (const auto& s) { this->init_window_size(s); }, size);

        int screen_index = 0;
        connection_ = {xcb_connect(nullptr, &screen_index), &xcb_disconnect};
        handle_ = xcb_generate_id(connection_.get());

        if (xcb_connection_has_error(connection_.get()))
        {
            throw std::runtime_error("Connection to xcb failed"); // TODO log error
        }

        xcb_errors_context_t* errors_ctx = nullptr;
        if (xcb_errors_context_new(connection_.get(), &errors_ctx) == 0)
        {
            errors_ctx_ = {errors_ctx, &xcb_errors_context_free};
        }
        else
        {
            throw std::runtime_error("Failed to initialize xcb-errors"); // TODO log error
        }

        const xcb_setup_t* const setup = xcb_get_setup(connection_.get());

        init_key_mapping(*setup);

        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

        while (screen_index-- > 0)
        {
            xcb_screen_next(&screen_iterator);
        }

        const xcb_screen_t* const screen = screen_iterator.data;

        // NOTE: order of values must be the same as order of values in xcb_cw_t enum
        const uint32_t value_list[] =
        {
            *reinterpret_cast<const uint32_t*>(background_color.data()),

            XCB_BACKING_STORE_WHEN_MAPPED,

            // TODO: configure by option
            XCB_EVENT_MASK_EXPOSURE |
            XCB_EVENT_MASK_STRUCTURE_NOTIFY |

            XCB_EVENT_MASK_BUTTON_PRESS |
            XCB_EVENT_MASK_BUTTON_RELEASE |

            XCB_EVENT_MASK_POINTER_MOTION |

            XCB_EVENT_MASK_BUTTON_MOTION |
            XCB_EVENT_MASK_BUTTON_1_MOTION |
            XCB_EVENT_MASK_BUTTON_2_MOTION |
            XCB_EVENT_MASK_BUTTON_3_MOTION |

            XCB_EVENT_MASK_KEY_PRESS |
            XCB_EVENT_MASK_KEY_RELEASE |

            XCB_EVENT_MASK_ENTER_WINDOW |
            XCB_EVENT_MASK_LEAVE_WINDOW
        };

        constexpr int16_t x = 20;
        constexpr int16_t y = 20;
        constexpr uint16_t border_width = 0;

        xcb_create_window
        (
            connection_.get(),
            XCB_COPY_FROM_PARENT,
            handle_,
            screen->root,
            x,
            y,
            current_size_.width,
            current_size_.height,
            border_width,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            screen->root_visual,
            XCB_CW_BACK_PIXEL |
            XCB_CW_BACKING_STORE |
            XCB_CW_EVENT_MASK,
            value_list
        );

        xcb_flush(connection_.get());

        constexpr std::string_view title{"window"};
        set_window_title(title);

        delete_reply_ = subscribe_to_close_event(*connection_, handle_);

        std::visit([this] (const auto& s) { this->init_window_size_constraints(s); }, size);

        xcb_flush(connection_.get());
    }

    WindowXCB::~WindowXCB()
    {
        xcb_destroy_window(connection_.get(), handle_); // TODO: use smart handler for this
    }

    vk::UniqueSurfaceKHR WindowXCB::create_surface(const vk::Instance& instance)
    {
        vk::XcbSurfaceCreateInfoKHR create_info(vk::XcbSurfaceCreateFlagsKHR(), connection_.get(), handle_);

        return vk::UniqueSurfaceKHR(instance.createXcbSurfaceKHRUnique(create_info));
    }

    void WindowXCB::start_display()
    {
        xcb_map_window(connection_.get(), handle_);
        xcb_flush(connection_.get());

        auto wait_event = [this] (const int event_type)
        {
            while (true)
            {
                const std::unique_ptr<xcb_generic_event_t, void(*)(void*)> event{xcb_wait_for_event(connection_.get()), &free};
                const auto received_event_type = event->response_type & ~0x80;
                if (received_event_type == event_type)
                {
                    break;
                }
            }
        };

        wait_event(XCB_MAP_NOTIFY);
        wait_event(XCB_EXPOSE);
        wait_event(XCB_CONFIGURE_NOTIFY);
    }

    void WindowXCB::set_window_title(const std::string_view title)
    {
        xcb_change_property
        (
            connection_.get(),
            XCB_PROP_MODE_REPLACE,
            handle_,
            XCB_ATOM_WM_NAME,
            XCB_ATOM_STRING,
            WindowPropertyFormat::BIT_8,
            static_cast<uint32_t>(title.length()),
            title.data()
        );
        xcb_flush(connection_.get());
    }

    std::vector<WindowEvent> WindowXCB::grab_events()
    {
        std::vector<WindowEvent> events;

        std::optional<WindowEventResize> last_resize_event;
        std::optional<std::string> error_message;

        std::unique_ptr<xcb_generic_event_t, void(*)(void*)> event{xcb_poll_for_event(connection_.get()), &free};
        while (event != nullptr)
        {
            switch (event->response_type & 0x7f)
            {
            case 0:
            {
                const auto error_event = reinterpret_cast<const xcb_generic_error_t*>(event.get());
                error_message.emplace(error_event_to_string(*errors_ctx_, *error_event));
                break;
            }
            case XCB_EXPOSE:
            {
                [[ maybe_unused ]] const auto expose_event = reinterpret_cast<const xcb_expose_event_t*>(event.get());
                events.emplace_back(WindowExposed{});
                break;
            }
            case XCB_CLIENT_MESSAGE:
            {
                assert(delete_reply_ != nullptr);
                if (reinterpret_cast<xcb_client_message_event_t*>(event.get())->data.data32[0] == delete_reply_->atom)
                {
                    delete_reply_ = {nullptr, &free};
                    events.emplace_back(WindowEventClose{});
                }
                break;
            }
            case XCB_CONFIGURE_NOTIFY:
            {
                const auto resize_event = reinterpret_cast<const xcb_configure_notify_event_t*>(event.get());
                if
                (
                    resize_event->width != current_size_.width
                    or resize_event->height != current_size_.height
                )
                {
                    current_size_.width = resize_event->width;
                    current_size_.height = resize_event->height;
                    if (current_size_.width > 0 and current_size_.height > 0)
                    {
                        last_resize_event = WindowEventResize{current_size_};
                    }
                }
                break;
            }
            case XCB_BUTTON_PRESS:
            {
                const auto button_press_event = reinterpret_cast<const xcb_button_press_event_t*>(event.get());
                if
                (
                    auto converted_event = handle_mouse_button_event<ButtonEvent::PRESS>(*button_press_event);
                    converted_event.has_value()
                )
                {
                    events.emplace_back(*std::move(converted_event));
                }
                break;
            }
            case XCB_BUTTON_RELEASE:
            {
                const auto button_release_event = reinterpret_cast<const xcb_button_release_event_t*>(event.get());
                if
                (
                    auto converted_event = handle_mouse_button_event<ButtonEvent::RELEASE>(*button_release_event);
                    converted_event.has_value()
                )
                {
                    events.emplace_back(*std::move(converted_event));
                }
                break;
            }
            case XCB_MOTION_NOTIFY:
            {
                const auto motion_event = reinterpret_cast<const xcb_motion_notify_event_t*>(event.get());
                events.emplace_back(handle_mouse_motion_event(*motion_event));
                break;
            }
            case XCB_ENTER_NOTIFY:
            {
                const auto enter_event = reinterpret_cast<const xcb_enter_notify_event_t*>(event.get());
                events.emplace_back(handle_cross_window_border_event<CrossEvent::ENTER>(*enter_event));
                break;
            }
            case XCB_LEAVE_NOTIFY:
            {
                const auto leave_event = reinterpret_cast<const xcb_leave_notify_event_t*>(event.get());
                events.emplace_back(handle_cross_window_border_event<CrossEvent::LEAVE>(*leave_event));
                break;
            }
            case XCB_KEY_PRESS:
            {
                assert(key_syms_ != nullptr);
                auto key_press_event = reinterpret_cast<xcb_key_press_event_t*>(event.get());
                events.emplace_back(handle_key_event<ButtonEvent::PRESS>(*key_syms_, *key_press_event));
                break;
            }
            case XCB_KEY_RELEASE:
            {
                assert(key_syms_ != nullptr);
                auto key_release_event = reinterpret_cast<xcb_key_release_event_t*>(event.get());
                events.emplace_back(handle_key_event<ButtonEvent::PRESS>(*key_syms_, *key_release_event));
                break;
            }
            }

            if (error_message.has_value())
            {
                throw std::runtime_error(*std::move(error_message)); // TODO log error
            }

            event = {xcb_poll_for_event(connection_.get()), &free};
        }

        if (last_resize_event.has_value())
        {
            events.emplace_back(*last_resize_event);
            last_resize_event.reset();
        }

        return events;
    }

    void WindowXCB::resize(const Size& size)
    {
        if (size.width != current_size_.width or size.height != current_size_.height)
        {
            const uint32_t values[] = {size.width, size.height};
            xcb_configure_window
            (
                connection_.get(),
                handle_,
                XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
                values
            );
            xcb_flush(connection_.get());
            xcb_aux_sync(connection_.get());
        }
    }
}
