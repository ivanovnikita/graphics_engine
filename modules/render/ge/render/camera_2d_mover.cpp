#include "camera_2d_mover.h"

namespace ge
{
    Camera2dMover::Camera2dMover(WithCamera2dI& render)
        : render_{render}
    {
    }

    void Camera2dMover::zoom(const glm::vec2& zoom_point, const ZoomDirection zoom_direction)
    {
        Camera2d camera = render_.get().get_camera();

        const glm::vec2 normalized_event_pos = camera.normalize_in_proj_space(zoom_point);
        const glm::vec2 model_space_event_pos = camera.proj_to_model_space(normalized_event_pos);

        constexpr float SCALE_STEP = 0.1f;
        float new_scale = camera.get_scale();
        switch (zoom_direction)
        {
        case ZoomDirection::In:
        {
            new_scale *= (1 - SCALE_STEP);
            break;
        }
        case ZoomDirection::Out:
        {
            new_scale *= (1 + SCALE_STEP);
            break;
        }
        }
        camera.set_scale(new_scale);

        // We know that mouse cursor must save the same coords in model and projection space,
        // so we jsut need to calculate new camera position.
        const glm::vec2 new_camera_pos = model_space_event_pos -
                (camera.proj_to_model_space(normalized_event_pos) - camera.get_pos());
        camera.set_pos(new_camera_pos);

        render_.get().set_camera(std::move(camera));
    }

    void Camera2dMover::drag_move_start(const glm::vec2& drag_point)
    {
        const Camera2d& camera = render_.get().get_camera();
        last_drag_point_ = camera.proj_to_model_space(camera.normalize_in_proj_space(drag_point));
    }

    void Camera2dMover::drag_move(const glm::vec2& drag_point)
    {
        if (not last_drag_point_.has_value())
        {
            drag_move_start(drag_point);
            return;
        }

        Camera2d camera = render_.get().get_camera();

        const glm::vec2 normalized_mouse_pos = camera.normalize_in_proj_space(drag_point);
        const glm::vec2 mouse_pos = camera.proj_to_model_space(normalized_mouse_pos);
        const glm::vec2 mouse_pos_delta = mouse_pos - *last_drag_point_;

        camera.set_pos(camera.get_pos() - mouse_pos_delta);

        last_drag_point_ = camera.proj_to_model_space(normalized_mouse_pos);

        render_.get().set_camera(std::move(camera));
    }

    void Camera2dMover::drag_move_end()
    {
        last_drag_point_.reset();
    }
}
