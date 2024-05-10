#include "render_2d_loop.h"

namespace ge
{
    namespace
    {
        ZoomDirection map_zoom_direction(const ScrollButton direction)
        {
            switch (direction)
            {
            case ScrollButton::UP: return ZoomDirection::In;
            case ScrollButton::DOWN: return ZoomDirection::Out;
            }

            __builtin_unreachable();
        }
    }

    template <typename T>
    void Render2dLoop::handle_window_event(const T&)
    {
    }

    template <>
    void Render2dLoop::handle_window_event(const WindowExposed&)
    {
        need_redraw_= NeedRedraw::Yes;
    }

    template <>
    void Render2dLoop::handle_window_event(const WindowEventClose&)
    {
        stopped_ = true;
    }

    template <>
    void Render2dLoop::handle_window_event(const WindowEventResize& event)
    {
        render_.resize(Extent<uint32_t>{.width = event.new_size.width, .height = event.new_size.height});
        need_redraw_= NeedRedraw::Yes;
    }

    template <>
    void Render2dLoop::handle_window_event(const WheelEvent& event)
    {
        camera_mover_.zoom(event.pos, map_zoom_direction(event.direction));
        need_redraw_= NeedRedraw::Yes;

        deactivate_state(InputState::CameraDragMove);
    }

    template <>
    void Render2dLoop::handle_window_event(const MouseButtonPress& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_press_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            world2d::MouseButtonPress event_model_space_pos
            {
                .pos = camera.to_world_space(camera.normalize(event.pos)),
                .button = event.button
            };
            combine_need_redraw(mouse_press_callback_(event_model_space_pos));
        }

        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            camera_mover_.drag_move_start(event.pos);
            active_states_.set(InputState::CameraDragMove);
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
    void Render2dLoop::handle_window_event(const MouseButtonRelease& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_release_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            world2d::MouseButtonRelease event_model_space_pos
            {
                .pos = camera.to_world_space(camera.normalize(event.pos)),
                .button = event.button
            };
            combine_need_redraw(mouse_release_callback_(event_model_space_pos));
        }

        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            deactivate_state(InputState::CameraDragMove);
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
    void Render2dLoop::handle_window_event(const MouseMoveEvent& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_move_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            world2d::MouseMoveEvent event_model_space_pos
            {
                .pos = camera.to_world_space(camera.normalize(event.pos))
            };
            combine_need_redraw(mouse_move_callback_(event_model_space_pos));
        }

        if (active_states_.test(InputState::CameraDragMove))
        {
            camera_mover_.drag_move(event.pos);
            need_redraw_= NeedRedraw::Yes;
        }
    }

    template <>
    void Render2dLoop::handle_window_event(const MouseEnterWindow&)
    {
    }

    template <>
    void Render2dLoop::handle_window_event(const MouseLeaveWindow&)
    {
        deactivate_state(InputState::CameraDragMove);
    }
    
    Render2dLoop::Render2dLoop
    (
        WindowI& window,
        Render2dI& render
    )
        : window_{window}
        , render_{render}
        , stopped_{false}
        , camera_mover_{render_}
        , need_redraw_{NeedRedraw::No}
    {
    }
    
    bool Render2dLoop::stopped() const
    {
        return stopped_;
    }
    
    void Render2dLoop::handle_window_events()
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
    
    void Render2dLoop::combine_need_redraw(NeedRedraw v)
    {
        need_redraw_ = ::ge::combine_need_redraw(need_redraw_, v);
    }
    
    void Render2dLoop::deactivate_state(const InputState state)
    {
        switch (state)
        {
        case InputState::CameraDragMove:
        {
            if (active_states_.test(InputState::CameraDragMove))
            {
                camera_mover_.drag_move_end();
                active_states_.reset(InputState::CameraDragMove);
            }
            break;
        }
        }
    }
    
    void Render2dLoop::set_mouse_press_callback(MouseButtonPressCallback callback)
    {
        mouse_press_callback_ = std::move(callback);
    }
    
    void Render2dLoop::set_mouse_release_callback(MouseButtonReleaseCallback callback)
    {
        mouse_release_callback_ = std::move(callback);
    }
    
    void Render2dLoop::set_mouse_move_callback(MouseMoveCallback callback)
    {
        mouse_move_callback_ = std::move(callback);
    }
}
