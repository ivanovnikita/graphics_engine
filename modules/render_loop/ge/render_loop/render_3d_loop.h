#pragma once

#include "ge/render/camera/camera_3d_mover_i.h"
#include "ge/render/render_i.h"
#include "ge/window/window_i.h"

#include <optional>
#include <functional>

namespace ge
{
    class Render3dLoop final
    {
    public:
        explicit Render3dLoop
        (
            WindowI&,
            Render3dI&,
            Camera3dMoverI&
        );

        bool stopped() const;
        void handle_window_events();

    private:
        template <typename T>
        void handle_window_event(const T&);

        void combine_need_redraw(NeedRedraw);

        WindowI& window_;
        Render3dI& render_;
        bool stopped_;

        Camera3dMoverI& camera_mover_;

        NeedRedraw need_redraw_;
    };
}
