#include "render_3d_loop.h"

#include <iostream>

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

    template <>
    void Render3dLoop::handle_window_event(const ServiceKeyPress& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const ServiceKeyRelease& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const LatinKeyPress& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const LatinKeyRelease& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const MouseMoveEvent& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const MouseButtonPress& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    template <>
    void Render3dLoop::handle_window_event(const MouseButtonRelease& e)
    {
        combine_need_redraw(camera_mover_.handle_window_event(e));
    }

    Render3dLoop::Render3dLoop
    (
        WindowI& window,
        Render3dI& render,
        Camera3dMoverI& camera_mover
    )
        : window_{window}
        , render_{render}
        , stopped_{false}
        , camera_mover_{camera_mover}
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

        const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

        if (prev_timestamp_.has_value())
        {
            // TODO: а что если прошло слишком много времени?
            const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - *prev_timestamp_);
            combine_need_redraw(camera_mover_.move_camera(delta));
        }

        prev_timestamp_ = now;

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
        need_redraw_ = ::ge::combine_need_redraw(need_redraw_, v);
    }
}
