#pragma once

#include "ge/common/flags.hpp"
#include "ge/render/render_i.h"
#include "ge/render/camera_2d_mover.h"
#include "ge/window/window_i.h"

#include <optional>
#include <functional>

namespace ge
{
    enum class InputState
    {
        CameraDragMove = 1
    };

    class RenderLoop final
    {
    public:
        enum class NeedRedraw : uint8_t
        {
            No = 0,
            Yes = 1
        };

        using MouseButtonPressCallback = std::function<NeedRedraw(const MouseButtonPress&)>;
        using MouseButtonReleaseCallback = std::function<NeedRedraw(const MouseButtonRelease&)>;
        using MouseMoveCallback = std::function<NeedRedraw(const MouseMoveEvent&)>;

        explicit RenderLoop
        (
            WindowI&,
            Render2dI&
        );

        bool stopped() const;
        void handle_window_events();

        void set_mouse_press_callback(MouseButtonPressCallback);
        void set_mouse_release_callback(MouseButtonReleaseCallback);
        void set_mouse_move_callback(MouseMoveCallback);

    private:
        using ProjVec2 = glm::vec2;
        using ModelVec2 = glm::vec2;

        template <typename T>
        void handle_window_event(const T&);

        void combine_need_redraw(NeedRedraw);

        void deactivate_state(InputState);

        WindowI& window_;
        Render2dI& render_;
        bool stopped_;

        Flags<InputState> active_states_;

        Camera2dMover camera_mover_;

        NeedRedraw need_redraw_;

        MouseButtonPressCallback mouse_press_callback_;
        MouseButtonReleaseCallback mouse_release_callback_;
        MouseMoveCallback mouse_move_callback_;
    };
}
