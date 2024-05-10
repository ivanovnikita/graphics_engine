#pragma once

#include "camera_3d_mover_i.h"
#include "fps_action.h"
#include "fps_move_direction.h"
#include "keyboard.h"

#include <glm/ext/quaternion_float.hpp>

#include <optional>
#include <unordered_map>

namespace ge
{
    class Camera3dMoverFps final : public Camera3dMoverI
    {
    public:
        explicit Camera3dMoverFps(WithCamera3dI&);
        ~Camera3dMoverFps() override;

        NeedRedraw handle_window_event(const ServiceKeyPress&) override;
        NeedRedraw handle_window_event(const ServiceKeyRelease&) override;
        NeedRedraw handle_window_event(const LatinKeyPress&) override;
        NeedRedraw handle_window_event(const LatinKeyRelease&) override;
        NeedRedraw handle_window_event(const MouseButtonPress&) override;
        NeedRedraw handle_window_event(const MouseButtonRelease&) override;
        NeedRedraw handle_window_event(const MouseMoveEvent&) override;

        void look_at(const World3dCoords&);

        NeedRedraw move_camera(const std::chrono::milliseconds& delta_time) override;

    private:
        NeedRedraw handle_key_press(KeyboardKey);
        NeedRedraw handle_key_release(KeyboardKey);

        void angles_from_look_at(const glm::vec3&);
        void update_camera();

        std::unordered_map<KeyboardKey, FpsAction> mapping_actions_;
        FpsMoveDirection move_direction_state;
        float move_speed_;
        float rotate_speed_;
        std::optional<SurfaceCoords> prev_mouse_pos_; // TODO: set initial mouse pos
        float angle_h_;
        float angle_v_;
    };
}
