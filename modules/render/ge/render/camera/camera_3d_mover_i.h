#pragma once

#include "ge/render/direction.h"
#include "ge/render/render_i.h"
#include "ge/window/window_events.h"

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
        virtual NeedRedraw handle_window_event(const LatinKeyPress&) = 0;
        virtual NeedRedraw handle_window_event(const MouseButtonPress&) = 0;
        virtual NeedRedraw handle_window_event(const MouseButtonRelease&) = 0;
        virtual NeedRedraw handle_window_event(const MouseMoveEvent&) = 0;

    protected:
        void move(ScreenDirection, float delta);

        std::reference_wrapper<WithCamera3dI> render_;
    };
}
