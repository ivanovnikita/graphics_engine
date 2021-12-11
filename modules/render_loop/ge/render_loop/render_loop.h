#pragma once

#include "ge/render/render_i.h"
#include "ge/window/window_i.h"

#include <optional>
#include <functional>

namespace ge
{
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

        RenderLoop(WindowI& window, RenderI& render);

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

        WindowI& window_;
        RenderI& render_;
        bool stopped_;

        std::optional<ProjVec2> prev_move_mouse_pos_;
        NeedRedraw need_redraw_;

        MouseButtonPressCallback mouse_press_callback_;
        MouseButtonReleaseCallback mouse_release_callback_;
        MouseMoveCallback mouse_move_callback_;
    };
}
