#include "render_loop.h"

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
    void RenderLoop::handle_window_event(const T&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const WindowExposed&)
    {
        need_redraw_= NeedRedraw::Yes;
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
        need_redraw_= NeedRedraw::Yes;
    }

    template <>
    void RenderLoop::handle_window_event(const WheelEvent& event)
    {
        camera_mover_.zoom(event.pos, map_zoom_direction(event.direction));
        need_redraw_= NeedRedraw::Yes;

        deactivate_state(InputState::CameraDragMove);
    }

    template <>
    void RenderLoop::handle_window_event(const MouseButtonPress& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_press_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            MouseButtonPress event_model_space_pos = event;
            event_model_space_pos.pos = camera.proj_to_model_space(camera.normalize_in_proj_space(event.pos));
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
    void RenderLoop::handle_window_event(const MouseButtonRelease& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_release_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            MouseButtonRelease event_model_space_pos = event;
            event_model_space_pos.pos = camera.proj_to_model_space(camera.normalize_in_proj_space(event.pos));
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
    void RenderLoop::handle_window_event(const MouseMoveEvent& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_move_callback_ != nullptr)
        {
            const Camera2d& camera = render_.get_camera();
            MouseMoveEvent event_model_space_pos = event;
            event_model_space_pos.pos = camera.proj_to_model_space(camera.normalize_in_proj_space(event.pos));
            combine_need_redraw(mouse_move_callback_(event_model_space_pos));
        }

        if (active_states_.test(InputState::CameraDragMove))
        {
            camera_mover_.drag_move(event.pos);
            need_redraw_= NeedRedraw::Yes;
        }
    }

    template <>
    void RenderLoop::handle_window_event(const MouseEnterWindow&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseLeaveWindow&)
    {
        deactivate_state(InputState::CameraDragMove);
    }

    RenderLoop::RenderLoop(WindowI& window, RenderI& render)
        : window_{window}
        , render_{render}
        , stopped_{false}
        , camera_mover_{render_}
        , need_redraw_{NeedRedraw::No}
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

    void RenderLoop::combine_need_redraw(NeedRedraw v)
    {
        need_redraw_ = static_cast<NeedRedraw>(static_cast<uint8_t>(need_redraw_) | static_cast<uint8_t>(v));
    }

    void RenderLoop::deactivate_state(const InputState state)
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

    void RenderLoop::set_mouse_press_callback(MouseButtonPressCallback callback)
    {
        mouse_press_callback_ = std::move(callback);
    }

    void RenderLoop::set_mouse_release_callback(MouseButtonReleaseCallback callback)
    {
        mouse_release_callback_ = std::move(callback);
    }

    void RenderLoop::set_mouse_move_callback(MouseMoveCallback callback)
    {
        mouse_move_callback_ = std::move(callback);
    }
}
