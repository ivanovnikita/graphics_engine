module;

#include <glm/glm.hpp>

#include <utility>

module camera_2d_mover;

namespace ge
{
    Camera2dMover::Camera2dMover(WithCamera2dI& render)
        : render_{render}
    {
    }

    void Camera2dMover::zoom(const SurfaceCoords& zoom_point, const ZoomDirection zoom_direction)
    {
        Camera2d camera = render_.get().get_camera();

        const SurfaceNormalizedCoords normalized_event_pos = camera.normalize(zoom_point);
        const World2dCoords world_space_event_pos = camera.to_world_space(normalized_event_pos);

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

        // Mouse cursor must save the same coords in world and projection space,
        // so we need to calculate new camera position.
        const World2dCoords new_camera_pos
        {
            world_space_event_pos.coords -
                (camera.to_world_space(normalized_event_pos).coords - camera.get_pos().coords)
        };
        camera.set_pos(new_camera_pos);

        render_.get().set_camera(std::move(camera));
    }

    void Camera2dMover::drag_move_start(const SurfaceCoords& drag_point)
    {
        const Camera2d& camera = render_.get().get_camera();
        last_drag_point_ = camera.to_world_space(camera.normalize(drag_point));
    }

    void Camera2dMover::drag_move(const SurfaceCoords& drag_point)
    {
        if (not last_drag_point_.has_value())
        {
            drag_move_start(drag_point);
            return;
        }

        Camera2d camera = render_.get().get_camera();

        const SurfaceNormalizedCoords normalized_mouse_pos = camera.normalize(drag_point);
        const World2dCoords mouse_pos = camera.to_world_space(normalized_mouse_pos);
        const World2dCoords mouse_pos_delta{mouse_pos.coords - last_drag_point_->coords};

        camera.set_pos(World2dCoords{camera.get_pos().coords - mouse_pos_delta.coords});

        last_drag_point_ = camera.to_world_space(normalized_mouse_pos);

        render_.get().set_camera(std::move(camera));
    }

    void Camera2dMover::drag_move_end()
    {
        last_drag_point_.reset();
    }
}
