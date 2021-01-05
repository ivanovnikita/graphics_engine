#pragma once

#include <glm/vec2.hpp>

#include <optional>
#include <functional>

namespace ge
{
    class Window;
    class Render;

    class RenderLoop final
    {
    public:
        using MouseMoveCallback = std::function<void(const glm::vec2& /*new mouse pos in model space*/)>;

        RenderLoop(ge::Window& window, ge::Render& render);

        bool stopped() const;
        void handle_window_events();

        void set_mouse_move_callback(MouseMoveCallback);

    private:
        using ProjVec2 = glm::vec2;
        using ModelVec2 = glm::vec2;

        template <typename T>
        void handle_window_event(const T&);

        ge::Window& window_;
        ge::Render& render_;
        bool stopped_;

        std::optional<ProjVec2> prev_move_mouse_pos_;
        bool need_draw_;

        MouseMoveCallback mouse_move_callback_;
    };
}
