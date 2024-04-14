module;

#include <variant>
#include <vector>

module render_3d_loop;

namespace ge
{
    template <typename T>
    void Render3dLoop::handle_window_event(const T&)
    {
    }

    template <>
    void Render3dLoop::handle_window_event(const WindowExposed&)
    {
        need_redraw_= NeedRedraw::Yes;
    }

    template <>
    void Render3dLoop::handle_window_event(const WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void Render3dLoop::handle_window_event(const WindowEventResize& event)
    {
        render_.resize(Extent<uint32_t>{.width = event.new_size.width, .height = event.new_size.height});
        need_redraw_= NeedRedraw::Yes;
    }

    Render3dLoop::Render3dLoop
    (
        WindowI& window,
        Render3dI& render
    )
        : window_{window}
        , render_{render}
        , stopped_{false}
        , need_redraw_{NeedRedraw::No}
    {
    }
    
    bool Render3dLoop::stopped() const
    {
        return stopped_;
    }
    
    void Render3dLoop::handle_window_events()
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

        switch (need_redraw_)
        {
        case NeedRedraw::Yes:
        {
            render_.draw_frame();
            need_redraw_ = NeedRedraw::No;
            break;
        }
        case NeedRedraw::No:
        {
            break;
        }
        }
    }
    
    void Render3dLoop::combine_need_redraw(NeedRedraw v)
    {
        need_redraw_ = static_cast<NeedRedraw>(static_cast<uint8_t>(need_redraw_) | static_cast<uint8_t>(v));
    }
}
