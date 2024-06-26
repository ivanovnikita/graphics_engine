#include "camera_3d_mover_fps.h"

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <iostream>

namespace ge
{
    Camera3dMoverFps::Camera3dMoverFps(WithCamera3dI& render)
        : Camera3dMoverI{render}
        , mapping_actions_
        {
            {KeyboardKey::Up, FpsAction::MoveCamera_Forward},
            {KeyboardKey::W, FpsAction::MoveCamera_Forward},
            {KeyboardKey::Down, FpsAction::MoveCamera_Backward},
            {KeyboardKey::S, FpsAction::MoveCamera_Backward},
            {KeyboardKey::Left, FpsAction::MoveCamera_Left},
            {KeyboardKey::A, FpsAction::MoveCamera_Left},
            {KeyboardKey::Right, FpsAction::MoveCamera_Right},
            {KeyboardKey::D, FpsAction::MoveCamera_Right},
            {KeyboardKey::E, FpsAction::MoveCamera_Up},
            {KeyboardKey::Q, FpsAction::MoveCamera_Down},
        }
        , move_speed_{0.001f}
        , rotate_speed_{0.2f}
        , angle_h_{0.f}
        , angle_v_{0.f}
    {
        look_at(World3dCoords{{0.f, 0.f, -1.f}});
    }

    Camera3dMoverFps::~Camera3dMoverFps() = default;

    // TODO: сохранять нажатые кнопки в общую структуру Keyboard.
    // Затем, независимо от приходящих event'ов, с заданной частотой проверять
    // в Keyboard нажатые кнопки и реагировать соответствующим образом.
    // Это позволит перемещать камеру по диагонали и сделать её смещение
    // более плавным по всем направлениям.

    NeedRedraw Camera3dMoverFps::handle_window_event(const ServiceKeyPress& e)
    {
        if (const std::optional<KeyboardKey> key = map(e.key); key.has_value())
        {
            handle_key_press(*key);
        }
        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const ServiceKeyRelease& e)
    {
        if (const std::optional<KeyboardKey> key = map(e.key); key.has_value())
        {
            handle_key_release(*key);
        }
        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const LatinKeyPress& e)
    {
        if (const std::optional<KeyboardKey> key = map(e.key); key.has_value())
        {
            handle_key_press(*key);
        }
        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const LatinKeyRelease& e)
    {
        if (const std::optional<KeyboardKey> key = map(e.key); key.has_value())
        {
            handle_key_release(*key);
        }
        return NeedRedraw::No;
    }

    // TODO: передавать delta_pos = speed * delta_time
    NeedRedraw Camera3dMoverFps::move_camera(const std::chrono::milliseconds& delta_time)
    {
        const std::optional<FpsAction> direction = move_direction_state.current_move_direction();
        if (not direction.has_value())
        {
            return NeedRedraw::No;
        }

        switch (*direction)
        {
        case FpsAction::MoveCamera_Forward:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos -= target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_Backward:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_Left:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& right = render_.get().get_camera().get_rotate_right();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += glm::normalize(-right) * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_Right:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& right = render_.get().get_camera().get_rotate_right();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += glm::normalize(right) * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_Down:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += glm::normalize(-up) * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_Up:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += glm::normalize(up) * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_ForwardLeft:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            const glm::quat rotate = glm::angleAxis(glm::radians(45.f), up);
            const glm::vec3 new_target = glm::normalize(rotate * target);
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos -= new_target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_ForwardRight:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            const glm::quat rotate = glm::angleAxis(glm::radians(-45.f), up);
            const glm::vec3 new_target = glm::normalize(rotate * target);
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos -= new_target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_BackwardLeft:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            const glm::quat rotate = glm::angleAxis(glm::radians(-45.f), up);
            const glm::vec3 new_target = glm::normalize(rotate * target);
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += new_target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        case FpsAction::MoveCamera_BackwardRight:
        {
            const float delta = move_speed_ * static_cast<float>(delta_time.count());
            const glm::vec3& target = render_.get().get_camera().get_rotate_target();
            const glm::vec3& up = render_.get().get_camera().get_rotate_up();
            const glm::quat rotate = glm::angleAxis(glm::radians(45.f), up);
            const glm::vec3 new_target = glm::normalize(rotate * target);
            glm::vec3 pos = render_.get().get_camera().get_pos().coords;
            pos += new_target * delta;
            render_.get().get_camera().set_pos(World3dCoords{std::move(pos)});
            break;
        }
        default: break;
        }


        return NeedRedraw::Yes;
    }

    NeedRedraw Camera3dMoverFps::handle_key_press(const KeyboardKey key)
    {
        if (const auto it = mapping_actions_.find(key); it != mapping_actions_.end())
        {
            move_direction_state.add(it->second);
        }
        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_key_release(const KeyboardKey key)
    {
        if (const auto it = mapping_actions_.find(key); it != mapping_actions_.end())
        {
            move_direction_state.sub(it->second);
        }
        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const MouseMoveEvent& e)
    {
        if (not prev_mouse_pos_.has_value())
        {
            return NeedRedraw::No;
        }

        const glm::vec2 delta = e.pos.coords - prev_mouse_pos_->coords;
        prev_mouse_pos_ = e.pos;

        angle_h_ -= delta.x * rotate_speed_;
        angle_v_ += delta.y * rotate_speed_;

        update_camera();

        return NeedRedraw::Yes;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const MouseButtonPress& e)
    {
        switch (e.button)
        {
        case MouseButton::LEFT:
        {
            if (prev_mouse_pos_.has_value())
            {
                prev_mouse_pos_.reset();
            }
            else
            {
                prev_mouse_pos_ = e.pos;
            }
            break;
        }
        case MouseButton::RIGHT:
        case MouseButton::MIDDLE:
        {
            break;
        }
        }

        return NeedRedraw::No;
    }

    NeedRedraw Camera3dMoverFps::handle_window_event(const MouseButtonRelease& e)
    {
        switch (e.button)
        {
        case MouseButton::LEFT:
        case MouseButton::RIGHT:
        case MouseButton::MIDDLE:
        {
            break;
        }
        }

        return NeedRedraw::No;
    }

    // https://ogldev.org/www/tutorial15/tutorial15.html
    // TODO: обработать проблемные случаи: https://stackoverflow.com/a/49824672
    void Camera3dMoverFps::angles_from_look_at(const glm::vec3& target)
    {
        const glm::vec3 normalized_target = glm::normalize(target);
        const glm::vec3 h_target{normalized_target.x, 0.f, normalized_target.z};

        if (h_target.z >= 0.f)
        {
            if (h_target.x >= 0.f)
            {
                angle_h_ = 360.f - glm::degrees(std::asin(h_target.z));
            }
            else
            {
                angle_h_ = 180.f + glm::degrees(std::asin(h_target.z));
            }
        }
        else
        {
            if (h_target.x >= 0.f)
            {
                angle_h_ = glm::degrees(std::asin(-h_target.z));
            }
            else
            {
                angle_h_ = 180.f - glm::degrees(std::asin(-h_target.z));
            }
        }

        angle_v_ = -glm::degrees(std::asin(normalized_target.y));
    }

    // https://ogldev.org/www/tutorial13/tutorial13.html
    // https://ogldev.org/www/tutorial14/tutorial14.html
    // https://ogldev.org/www/tutorial15/tutorial15.html
    void Camera3dMoverFps::update_camera()
    {
        const glm::vec3 v_axis{0.0f, 1.0f, 0.0f};

        // Rotate the view vector by the horizontal angle around the vertical axis
        glm::vec3 view{1.0f, 0.0f, 0.0f};
        view = glm::normalize(glm::angleAxis(glm::radians(angle_h_), v_axis) * view);

        // Rotate the view vector by the vertical angle around the horizontal axis
        const glm::vec3 h_axis = glm::normalize(glm::cross(v_axis, view));
        view = glm::normalize(glm::angleAxis(glm::radians(angle_v_), h_axis) * view);

        const glm::vec3 target = -view;
        const glm::vec3 right = glm::normalize(glm::cross(v_axis, target));
        const glm::vec3 up = glm::cross(target, right);
        render_.get().get_camera().set_rotate(up, right, target);
    }

    // https://ogldev.org/www/tutorial15/tutorial15.html
    void Camera3dMoverFps::look_at(const World3dCoords& target)
    {
        angles_from_look_at(target.coords);
        update_camera();
    }
}
