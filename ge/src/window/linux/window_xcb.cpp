#include "window_xcb.h"
#include "exception.h"

namespace ge::impl
{

    WindowXCB::WindowXCB(uint16_t width, uint16_t height)
        : width_ (width)
        , height_ (height)
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
          , static_cast<uint32_t>(title.size())
          , title.c_str()
        );
    }

    WindowXCB::~WindowXCB()
    {
        xcb_destroy_window(connection_, handle_);
        xcb_disconnect(connection_);
    }

    std::unique_ptr<vk::UniqueSurfaceKHR> WindowXCB::create_surface(const vk::UniqueInstance& instance)
    {
        vk::XcbSurfaceCreateInfoKHR create_info(vk::XcbSurfaceCreateFlagsKHR(), connection_, handle_);

        return std::make_unique<vk::UniqueSurfaceKHR>(instance->createXcbSurfaceKHRUnique(create_info));
    }

    std::pair<uint16_t, uint16_t> WindowXCB::extent() const
    {
        return {width_, height_};
    }

}
