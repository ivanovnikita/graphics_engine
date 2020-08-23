#include "ge/window/linux/window_xcb.h"

#include <xcb/xcb_icccm.h>
#include <xcb/xcb_aux.h>
#include <xcb/xinput.h>

#include <xcb/xcb_keysyms.h>

#include <X11/keysymdef.h>

#include <iostream>
#include <sstream>

namespace ge
{
    namespace
    {
        enum WindowPropertyFormat
        {
            BIT_8 = 8
            , BIT_16 = 16
            , BIT_32 = 32
        };

        enum InternAtom
        {
            CREATE_IF_NOT_EXISTS = 0
            , ONLY_IF_EXISTS = 1
        };

        xcb_intern_atom_reply_t& subscribe_to_close_event
        (
            xcb_connection_t& connection
            , const xcb_window_t& handle
        )
        {
            constexpr std::string_view name_protocols("WM_PROTOCOLS");
            const xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom
            (
                &connection
                , InternAtom::ONLY_IF_EXISTS
                , name_protocols.length()
                , name_protocols.data()
            );
            xcb_intern_atom_reply_t* const protocols_reply = xcb_intern_atom_reply
            (
                &connection
                , protocols_cookie
                , nullptr
            );
            constexpr std::string_view delete_window("WM_DELETE_WINDOW");
            const xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom
            (
                &connection
                , InternAtom::CREATE_IF_NOT_EXISTS
                , delete_window.length()
                , delete_window.data()
            );
            xcb_intern_atom_reply_t* delete_reply = xcb_intern_atom_reply
            (
                &connection
                , delete_cookie
                , nullptr
            );
            xcb_change_property
            (
                &connection
                , XCB_PROP_MODE_REPLACE
                , handle
                , protocols_reply->atom
                , 4
                , 32
                , 1
                , &delete_reply->atom
            );
            free(protocols_reply);

            return *delete_reply;
        }

        void set_min_max_sizes
        (
            xcb_connection_t& connection
            , const xcb_window_t& handle
            , const Size& current_size
            , const std::optional<Size>& min_size
            , const std::optional<Size>& max_size
        )
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

        ModifiersState parse_modifiers(uint16_t state)
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
            xcb_errors_context_t* errors_ctx,
            const xcb_generic_error_t& e
        )
        {
            std::string result{"X error: request="};

            const char* major = xcb_errors_get_name_for_major_code(errors_ctx, e.major_code);
            const char* minor = xcb_errors_get_name_for_minor_code(errors_ctx, e.major_code, e.minor_code);
            const char* extension = nullptr;
            const char* error = xcb_errors_get_name_for_error(errors_ctx, e.error_code, &extension);

            assert(major != nullptr);
            result += major;

            if (minor != nullptr)
            {
                result += "-";
                result += minor;
            }

            result += " (major ";
            result += e.major_code;
            result += ", minor ";
            result += e.minor_code;
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
        std::optional<WindowEvent> handle_mouse_button_event(const xcb_button_press_event_t& event)
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

        WindowEvent handle_mouse_motion_event(const xcb_motion_notify_event_t& event)
        {
            const glm::vec2 pos{event.event_x, event.event_y};
            const ModifiersState modifiers{parse_modifiers(event.state)};
            const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

            return MouseMoveEvent{pos, modifiers, timestamp};
        }

        template <CrossEvent cross_event>
        WindowEvent handle_cross_window_border_event(const xcb_enter_notify_event_t& event)
        {
            const glm::vec2 pos{event.event_x, event.event_y};
            const ModifiersState modifiers{parse_modifiers(event.state)};
            const EventTimestamp timestamp{std::chrono::milliseconds{event.time}};

            return MouseCrossWindowBorderEvent<cross_event>
            {
                pos
                , modifiers
                , timestamp
            };
        }

        void handle_key_press_event(const xcb_key_press_event_t& event)
        {
            std::cout << event.detail << std::endl;
        }
    }

    template <>
    void WindowXCB::init_window_size<StaticSize>(const StaticSize& size)
    {
        current_size_ = size;
    }

    template <>
    void WindowXCB::init_window_size<DynamicSize>(const DynamicSize& size)
    {
        current_size_ = size.default_size;
    }

    template <>
    void WindowXCB::init_window_size_constraints(const StaticSize& size)
    {
        set_min_max_sizes(*connection_, handle_, size, size, size);
    }

    template <>
    void WindowXCB::init_window_size_constraints(const DynamicSize& size)
    {
        if (size.min_size.has_value() or size.max_size.has_value())
        {
            set_min_max_sizes(*connection_, handle_, size.default_size, size.min_size, size.max_size);
        }
    }

    WindowXCB::WindowXCB
    (
        const WindowSize& size
        , const std::array<uint8_t, 4> background_color
    )
        : delete_reply_(nullptr)
    {
        std::visit([this] (const auto& s) { this->init_window_size(s); }, size);

        int screen_index = 0;
        connection_ = xcb_connect(nullptr, &screen_index);
        handle_ = xcb_generate_id(connection_);

        if (xcb_connection_has_error(connection_))
        {
            throw std::runtime_error("Connection to xcb failed");
        }

        if (xcb_errors_context_new(connection_, &errors_ctx_) != 0)
        {
            throw std::runtime_error("Failed to initialize xcb-errors");
        }

        const xcb_setup_t* const setup = xcb_get_setup(connection_);
        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

        while (screen_index-- > 0)
        {
            xcb_screen_next(&screen_iterator);
        }

        const xcb_screen_t* const screen = screen_iterator.data;

        // NOTE: order of values must be the same as order of values in xcb_cw_t enum
        const uint32_t value_list[] =
        {
            *reinterpret_cast<const uint32_t*>(background_color.data())

            , XCB_BACKING_STORE_WHEN_MAPPED

            // TODO: configure by option
            , XCB_EVENT_MASK_EXPOSURE
            | XCB_EVENT_MASK_STRUCTURE_NOTIFY

            | XCB_EVENT_MASK_BUTTON_PRESS
            | XCB_EVENT_MASK_BUTTON_RELEASE

            | XCB_EVENT_MASK_POINTER_MOTION

            | XCB_EVENT_MASK_BUTTON_MOTION
            | XCB_EVENT_MASK_BUTTON_1_MOTION
            | XCB_EVENT_MASK_BUTTON_2_MOTION
            | XCB_EVENT_MASK_BUTTON_3_MOTION

            | XCB_EVENT_MASK_KEY_PRESS
            | XCB_EVENT_MASK_KEY_RELEASE

            | XCB_EVENT_MASK_ENTER_WINDOW
            | XCB_EVENT_MASK_LEAVE_WINDOW
        };

        constexpr int16_t x = 20;
        constexpr int16_t y = 20;
        constexpr uint16_t border_width = 0;

        xcb_create_window
        (
            connection_
          , XCB_COPY_FROM_PARENT
          , handle_
          , screen->root
          , x
          , y
          , current_size_.width
          , current_size_.height
          , border_width
          , XCB_WINDOW_CLASS_INPUT_OUTPUT
          , screen->root_visual
          , XCB_CW_BACK_PIXEL
          | XCB_CW_BACKING_STORE
          | XCB_CW_EVENT_MASK
          , value_list
        );

        xcb_flush(connection_);

        constexpr std::string_view title{"window"};
        xcb_change_property
        (
            connection_
          , XCB_PROP_MODE_REPLACE
          , handle_
          , XCB_ATOM_WM_NAME
          , XCB_ATOM_STRING
          , WindowPropertyFormat::BIT_8
          , static_cast<uint32_t>(title.length())
          , title.data()
        );

        delete_reply_ = &subscribe_to_close_event(*connection_, handle_);

        std::visit([this] (const auto& s) { this->init_window_size_constraints(s); }, size);

        xcb_flush(connection_);
    }

    WindowXCB::~WindowXCB()
    {
        if (delete_reply_ != nullptr)
        {
            free(delete_reply_);
        }

        xcb_destroy_window(connection_, handle_);
        xcb_errors_context_free(errors_ctx_);
        xcb_disconnect(connection_);
    }

    vk::UniqueSurfaceKHR WindowXCB::create_surface(const vk::Instance& instance)
    {
        vk::XcbSurfaceCreateInfoKHR create_info(vk::XcbSurfaceCreateFlagsKHR(), connection_, handle_);

        return vk::UniqueSurfaceKHR(instance.createXcbSurfaceKHRUnique(create_info));
    }

    void WindowXCB::start_display()
    {
        xcb_map_window(connection_, handle_);
        xcb_flush(connection_);

        auto wait_event = [this] (const int event_type)
        {
            while (true)
            {
                xcb_generic_event_t* const event = xcb_wait_for_event(connection_);
                const auto received_event_type = event->response_type & ~0x80;
                free(event);
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

    std::vector<WindowEvent> WindowXCB::grab_events()
    {
        std::vector<WindowEvent> events;

        std::optional<WindowEventResize> last_resize_event;
        std::optional<std::string> error_message;

        xcb_generic_event_t* event = xcb_poll_for_event(connection_);
        while (event != nullptr)
        {
            switch (event->response_type & 0x7f)
            {
            case 0:
            {
                const auto error_event = reinterpret_cast<const xcb_generic_error_t*>(event);
                error_message.emplace(error_event_to_string(errors_ctx_, *error_event));
                break;
            }
            case XCB_EXPOSE:
            {
                [[ maybe_unused ]] const auto expose_event = reinterpret_cast<const xcb_expose_event_t*>(event);
                events.emplace_back(WindowExposed{});
                break;
            }
            case XCB_CLIENT_MESSAGE:
            {
                if (reinterpret_cast<xcb_client_message_event_t*>(event)->data.data32[0] == delete_reply_->atom)
                {
                    free(delete_reply_);
                    delete_reply_ = nullptr;
                    events.emplace_back(WindowEventClose{});
                }
                break;
            }
            case XCB_CONFIGURE_NOTIFY:
            {
                const auto resize_event = reinterpret_cast<const xcb_configure_notify_event_t*>(event);
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
                const auto button_press_event = reinterpret_cast<const xcb_button_press_event_t*>(event);
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
                const auto button_release_event = reinterpret_cast<const xcb_button_release_event_t*>(event);
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
                const auto motion_event = reinterpret_cast<const xcb_motion_notify_event_t*>(event);
                events.emplace_back(handle_mouse_motion_event(*motion_event));
                break;
            }
            case XCB_ENTER_NOTIFY:
            {
                const auto enter_event = reinterpret_cast<const xcb_enter_notify_event_t*>(event);
                events.emplace_back(handle_cross_window_border_event<CrossEvent::ENTER>(*enter_event));
                break;
            }
            case XCB_LEAVE_NOTIFY:
            {
                const auto leave_event = reinterpret_cast<const xcb_leave_notify_event_t*>(event);
                events.emplace_back(handle_cross_window_border_event<CrossEvent::LEAVE>(*leave_event));
                break;
            }
            case XCB_KEY_PRESS:
            {
                const auto key_release_event = reinterpret_cast<const xcb_key_release_event_t*>(event);
                handle_key_press_event(*key_release_event);
                break;
            }
            case XCB_KEY_RELEASE:
            {
                break;
            }
            }

            free(event);

            if (error_message.has_value())
            {
                throw std::runtime_error(*error_message);
            }

            event = xcb_poll_for_event(connection_);
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
                connection_
                , handle_
                , XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT
                , values
            );
            xcb_flush(connection_);
            xcb_aux_sync(connection_);
        }
    }
}
