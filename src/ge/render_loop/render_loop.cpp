#include "render_loop.h"

#include "ge/render/render.h"
#include "ge/window/window.h"

namespace ge
{
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

        prev_move_mouse_pos_.reset();
    }

    template <>
    void RenderLoop::handle_window_event(const WheelEvent& event)
    {
        const ProjVec2 normalized_event_pos = render_.normalize_in_proj_space(event.pos);
        const ModelVec2 model_space_event_pos = render_.proj_to_model_space(normalized_event_pos);

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

        // We know that mouse cursor must save the same coords in model and projection space,
        // so we jsut need to calculate new camera position.
        const ModelVec2 new_camera_pos = model_space_event_pos -
                (render_.proj_to_model_space(normalized_event_pos) - render_.camera_pos());
        render_.set_camera_pos(new_camera_pos);

        need_redraw_= NeedRedraw::Yes;

        prev_move_mouse_pos_.reset();
    }

    template <>
    void RenderLoop::handle_window_event(const MouseButtonPress& event)
    {
        // TODO: rewrite copy-pasted code
        if (mouse_press_callback_ != nullptr)
        {
            MouseButtonPress event_model_space_pos = event;
            event_model_space_pos.pos = render_.proj_to_model_space(render_.normalize_in_proj_space(event.pos));
            combine_need_redraw(mouse_press_callback_(event_model_space_pos));
        }

        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            prev_move_mouse_pos_ = render_.proj_to_model_space(render_.normalize_in_proj_space(event.pos));
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
            MouseButtonRelease event_model_space_pos = event;
            event_model_space_pos.pos = render_.proj_to_model_space(render_.normalize_in_proj_space(event.pos));
            combine_need_redraw(mouse_release_callback_(event_model_space_pos));
        }

        switch (event.button)
        {
        case MouseButton::LEFT:
        {
            prev_move_mouse_pos_.reset();
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
            MouseMoveEvent event_model_space_pos = event;
            event_model_space_pos.pos = render_.proj_to_model_space(render_.normalize_in_proj_space(event.pos));
            combine_need_redraw(mouse_move_callback_(event_model_space_pos));
        }

        if (not prev_move_mouse_pos_.has_value() or not event.modifiers.mouse_left)
        {
            prev_move_mouse_pos_.reset();
            return;
        }

        const ProjVec2 normalized_mouse_pos = render_.normalize_in_proj_space(event.pos);
        const ModelVec2 mouse_pos = render_.proj_to_model_space(normalized_mouse_pos);
        const ModelVec2 mouse_pos_delta = mouse_pos - *prev_move_mouse_pos_;

        render_.set_camera_pos(render_.camera_pos() - mouse_pos_delta);
        need_redraw_= NeedRedraw::Yes;

        prev_move_mouse_pos_ = render_.proj_to_model_space(normalized_mouse_pos);
    }

    template <>
    void RenderLoop::handle_window_event(const MouseEnterWindow&)
    {
    }

    template <>
    void RenderLoop::handle_window_event(const MouseLeaveWindow&)
    {
        prev_move_mouse_pos_.reset();
    }

    RenderLoop::RenderLoop(Window& window, Render& render)
        : window_(window)
        , render_(render)
        , stopped_(false)
        , need_redraw_(NeedRedraw::No)
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
