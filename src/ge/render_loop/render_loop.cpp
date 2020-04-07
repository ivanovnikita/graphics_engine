#include "render_loop.h"

#include "ge/render/render.h"
#include "ge/window/window.h"

namespace ge
{
    template <>
    void RenderLoop::handle_window_event<ge::WindowExposed>(const WindowExposed&)
    {
        render_.draw_frame();
    }

    template <>
    void RenderLoop::handle_window_event<ge::WindowEventClose>(const WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void RenderLoop::handle_window_event<ge::WindowEventResize>(const WindowEventResize& event)
    {
        render_.resize(event.new_size.width, event.new_size.height);
        render_.draw_frame();
    }

    template <>
    void RenderLoop::handle_window_event<ge::WheelEvent>(const WheelEvent& event)
    {
        constexpr float SCALE_STEP = 0.25;
        float new_scale = render_.camera_scale();

        switch (event.direction)
        {
        case WheelEvent::Direction::UP:
        {
            new_scale *= (1 - SCALE_STEP);
            break;
        }
        case WheelEvent::Direction::DOWN:
        {
            new_scale *= (1 + SCALE_STEP);
            break;
        }
        }

        render_.set_camera_scale(new_scale);
        render_.draw_frame();
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
