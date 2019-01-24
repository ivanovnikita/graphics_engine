#include "window_xcb.h"
#include "exception.h"
#include "utils/safe_cast.hpp"

namespace ge::impl
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
    }

    WindowXCB::WindowXCB(uint16_t width, uint16_t height)
        : width_ (width)
        , height_ (height)
        , delete_reply_(nullptr)
        , closed_(false)
    {
        int screen_index = 0;
        connection_ = xcb_connect(nullptr, &screen_index);
        handle_ = xcb_generate_id(connection_);

        if (connection_ == nullptr)
        {
            GE_THROW(window_error, "Connection to xcb failed");
        }

        const xcb_setup_t* const setup = xcb_get_setup(connection_);
        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

        while (screen_index-- > 0)
        {
            xcb_screen_next(&screen_iterator);
        }

        const xcb_screen_t* const screen = screen_iterator.data;

        const uint32_t value_list[] =
        {
            screen->white_pixel
            , XCB_EVENT_MASK_EXPOSURE
            | XCB_EVENT_MASK_KEY_PRESS
            | XCB_EVENT_MASK_STRUCTURE_NOTIFY
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
          , width_
          , height_
          , border_width
          , XCB_WINDOW_CLASS_INPUT_OUTPUT
          , screen->root_visual
          , XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK
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
          , safe_cast<uint32_t>(title.length())
          , title.data()
        );

        constexpr std::string_view name_protocols("WM_PROTOCOLS");
        const xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom
        (
            connection_
            , InternAtom::ONLY_IF_EXISTS
            , name_protocols.length()
            , name_protocols.data()
        );
        xcb_intern_atom_reply_t* const protocols_reply = xcb_intern_atom_reply
        (
            connection_
            , protocols_cookie
            , nullptr
        );
        constexpr std::string_view delete_window("WM_DELETE_WINDOW");
        const xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom
        (
            connection_
            , InternAtom::CREATE_IF_NOT_EXISTS
            , delete_window.length()
            , delete_window.data()
        );
        delete_reply_ = xcb_intern_atom_reply
        (
            connection_
            , delete_cookie
            , nullptr
        );
        xcb_change_property
        (
            connection_
            , XCB_PROP_MODE_REPLACE
            , handle_
            , protocols_reply->atom
            , 4
            , 32
            , 1
            , &delete_reply_->atom
        );
        free(protocols_reply);

        xcb_flush(connection_);
    }

    WindowXCB::~WindowXCB()
    {
        if (delete_reply_ != nullptr)
        {
            free(delete_reply_);
        }

        xcb_destroy_window(connection_, handle_);
        xcb_disconnect(connection_);
    }

    vk::UniqueSurfaceKHR WindowXCB::create_surface(const vk::Instance& instance)
    {
        vk::XcbSurfaceCreateInfoKHR create_info(vk::XcbSurfaceCreateFlagsKHR(), connection_, handle_);

        return vk::UniqueSurfaceKHR(instance.createXcbSurfaceKHRUnique(create_info));
    }

    vk::Extent2D WindowXCB::extent() const
    {
        return {width_, height_};
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

    void WindowXCB::process_events()
    {
        xcb_generic_event_t* const event = xcb_poll_for_event(connection_);

        if (event == nullptr)
        {
            return;
        }

        switch (event->response_type & 0x7f)
        {
        case XCB_CLIENT_MESSAGE:
        {
            if (reinterpret_cast<xcb_client_message_event_t*>(event)->data.data32[0] == delete_reply_->atom)
            {
                closed_ = true;
                free(delete_reply_);
                delete_reply_ = nullptr;
            }
            break;
        }
        }

        free(event);
    }

    bool WindowXCB::closed() const
    {
        return closed_;
    }
}
