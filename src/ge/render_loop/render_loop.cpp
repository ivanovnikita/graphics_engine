#include "render_loop.h"

#include "ge/render/render.h"
#include "ge/window/window.h"

namespace ge
{
    template <>
    void RenderLoop::handle_window_event<ge::WindowEventClose>(const ge::WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void RenderLoop::handle_window_event<ge::WindowEventResize>(const ge::WindowEventResize&)
    {
    }

    RenderLoop::RenderLoop(ge::Window& window, ge::Render& render)
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
        const std::vector<ge::WindowEvent> events = window_.grab_events();
        for (const auto& event : events)
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
