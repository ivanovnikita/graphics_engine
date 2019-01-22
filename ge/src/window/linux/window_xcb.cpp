#include "window_xcb.h"
#include "exception.h"
#include "utils/safe_cast.hpp"

namespace ge::impl
{

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

        const xcb_setup_t* setup = xcb_get_setup(connection_);
        xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

        while (screen_index-- > 0)
        {
            xcb_screen_next(&screen_iterator);
        }

        xcb_screen_t* screen = screen_iterator.data;

        uint32_t value_list[] =
        {
            screen->white_pixel,
            XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY
        };

        xcb_create_window
        (
            connection_
          , XCB_COPY_FROM_PARENT
          , handle_
          , screen->root
          , 20
          , 20
          , width_
          , height_
          , 0
          , XCB_WINDOW_CLASS_INPUT_OUTPUT
          , screen->root_visual
          , XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK
          , value_list
        );

        xcb_flush(connection_);

        std::string title("window");
        xcb_change_property
        (
            connection_
          , XCB_PROP_MODE_REPLACE
          , handle_
          , XCB_ATOM_WM_NAME
          , XCB_ATOM_STRING
          , 8
          , safe_cast<uint32_t>(title.size())
          , title.c_str()
        );
    }

    WindowXCB::~WindowXCB()
    {
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
        constexpr std::string_view name_protocols("WM_PROTOCOLS");
        const xcb_intern_atom_cookie_t protocols_cookie = xcb_intern_atom
        (
            connection_
            , 1
            , name_protocols.length()
            , name_protocols.data()
        );
        xcb_intern_atom_reply_t* protocols_reply = xcb_intern_atom_reply
        (
            connection_
            , protocols_cookie
            , nullptr
        );
        constexpr std::string_view delete_window("WM_DELETE_WINDOW");
        const xcb_intern_atom_cookie_t delete_cookie = xcb_intern_atom
        (
            connection_
            , 0
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

        xcb_map_window(connection_, handle_);
        xcb_flush(connection_);
    }

    void WindowXCB::process_events()
    {
        xcb_generic_event_t* event = xcb_poll_for_event(connection_);

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
    }

    bool WindowXCB::closed() const
    {
        return closed_;
    }

}
