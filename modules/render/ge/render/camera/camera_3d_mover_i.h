#pragma once

#include "ge/render/render_i.h"
#include "ge/window/window_events.h"

#include <chrono>
#include <functional>
#include <optional>

namespace ge
{
    class Camera3dMoverI
    {
    public:
        explicit Camera3dMoverI(WithCamera3dI&);
        virtual ~Camera3dMoverI();

        virtual NeedRedraw handle_window_event(const ServiceKeyPress&) = 0;
        virtual NeedRedraw handle_window_event(const ServiceKeyRelease&) = 0;
        virtual NeedRedraw handle_window_event(const LatinKeyPress&) = 0;
        virtual NeedRedraw handle_window_event(const LatinKeyRelease&) = 0;
        virtual NeedRedraw handle_window_event(const MouseButtonPress&) = 0;
        virtual NeedRedraw handle_window_event(const MouseButtonRelease&) = 0;
        virtual NeedRedraw handle_window_event(const MouseMoveEvent&) = 0;

        virtual NeedRedraw move_camera(const std::chrono::milliseconds& delta_time) = 0;

    protected:
        std::reference_wrapper<WithCamera3dI> render_;
    };
}
