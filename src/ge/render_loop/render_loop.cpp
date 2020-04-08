#include "render_loop.h"

#include "ge/render/render.h"
#include "ge/window/window.h"

namespace ge
{
    template <>
    void RenderLoop::handle_window_event(const WindowExposed&)
    {
        render_.draw_frame();
    }

    template <>
    void RenderLoop::handle_window_event(const WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void RenderLoop::handle_window_event(const WindowEventResize& event)
    {
        render_.resize(event.new_size.width, event.new_size.height);
        render_.draw_frame();
    }

    template <>
    void RenderLoop::handle_window_event(const WheelEvent& event)
    {
        constexpr float SCALE_STEP = 0.1f;
        float new_scale = render_.camera_scale();

        switch (event.direction)
        {
        case ScrollButton::UP:
        {
            new_scale *= (1 - SCALE_STEP);
            break;
        }
        case ScrollButton::DOWN:
        {
            new_scale *= (1 + SCALE_STEP);
            break;
        }
        }

        render_.set_camera_scale(new_scale);

        render_.draw_frame();
    }

    template <>
    void RenderLoop::handle_window_event(const MouseButtonPress& event)
    {
        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            break;
        }
        case MouseButton::RIGHT:
        {
            break;
        }
        case MouseButton::MIDDLE:
        {
            break;
        }
        }
    }

    template <>
    void RenderLoop::handle_window_event(const MouseButtonRelease& event)
    {
        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            break;
        }
        case MouseButton::RIGHT:
        {
            break;
        }
        case MouseButton::MIDDLE:
        {
            break;
        }
        }
    }

    template <>
    void RenderLoop::handle_window_event(const MouseMovePointerEvent&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseMovePressedLeftEvent&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseMovePressedRightEvent&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseMovePressedMiddleEvent&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseMovePressedManyEvent&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseEnterWindow&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseLeaveWindow&)
    {
    }

    RenderLoop::RenderLoop(Window& window, Render& render)
        : window_(window)
        , render_(render)
        , stopped_(false)
    {
    }

    bool RenderLoop::stopped() const
    {
        return stopped_;
    }

    void RenderLoop::handle_window_events()
    {
        const std::vector<WindowEvent> events = window_.grab_events();
        for (const WindowEvent& event : events)
        {
            std::visit
            (
                [this] (const auto& e)
                {
                    this->handle_window_event(e);
                }
                , event
            );
        }
    }
}
