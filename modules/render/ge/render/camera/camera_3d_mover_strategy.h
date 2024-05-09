#pragma once

#include "camera_3d_mover_i.h"

namespace ge
{
    class Camera3dMoverStrategy final : public Camera3dMoverI
    {
    public:
        explicit Camera3dMoverStrategy(WithCamera3dI&);
        ~Camera3dMoverStrategy() override;

        NeedRedraw handle_window_event(const ServiceKeyPress&) override;
        NeedRedraw handle_window_event(const LatinKeyPress&) override;
        NeedRedraw handle_window_event(const MouseButtonPress&) override;
        NeedRedraw handle_window_event(const MouseButtonRelease&) override;
        NeedRedraw handle_window_event(const MouseMoveEvent&) override;
    };
}
