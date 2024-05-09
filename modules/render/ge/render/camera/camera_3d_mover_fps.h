#pragma once

#include "camera_3d_mover_i.h"
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
        NeedRedraw handle_window_event(const LatinKeyPress&) override;
        NeedRedraw handle_window_event(const MouseButtonPress&) override;
        NeedRedraw handle_window_event(const MouseButtonRelease&) override;
        NeedRedraw handle_window_event(const MouseMoveEvent&) override;

        void look_at(const World3dCoords&);

    private:
        NeedRedraw handle_key_press(KeyboardKey);

        void angles_from_look_at(const glm::vec3&);
        void update_camera();

        std::unordered_map<KeyboardKey, ScreenDirection> mapping_move_;
        std::unordered_map<KeyboardKey, ScreenDirection> mapping_rotate_;
        float move_speed_;
        float rotate_speed_;
        std::optional<SurfaceCoords> prev_mouse_pos_; // TODO: set initial mouse pos
        float angle_h_;
        float angle_v_;
    };
}
