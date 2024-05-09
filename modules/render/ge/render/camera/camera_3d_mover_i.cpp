#include "camera_3d_mover_i.h"

#include <iostream>

namespace ge
{
    Camera3dMoverI::Camera3dMoverI(WithCamera3dI& render)
        : render_{render}
    {
    }

    Camera3dMoverI::~Camera3dMoverI() = default;

    void Camera3dMoverI::move(const ScreenDirection direction, const float delta)
    {
        World3dCoords pos = render_.get().get_camera().get_pos();

        glm::vec3 result_pos = pos.coords;
        const glm::vec3& right = render_.get().get_camera().get_rotate_right();
        const glm::vec3& target = render_.get().get_camera().get_rotate_target();
        switch (direction)
        {
        case ScreenDirection::forward:
        {
            result_pos -= target * delta;
            break;
        }
        case ScreenDirection::backward:
        {
            result_pos += target * delta;
            break;
        }
        case ScreenDirection::left:
        {
            glm::vec3 l = glm::normalize(-right);
            l *= delta;
            result_pos += l;
            break;
        }
        case ScreenDirection::right:
        {
            glm::vec3 r = glm::normalize(right);
            r *= delta;
            result_pos += r;
            break;
        }
        case ScreenDirection::down:
        {
            break;
        }
        case ScreenDirection::up:
        {
            break;
        }
        }

        render_.get().get_camera().set_pos(World3dCoords{result_pos});
    }
}
