#pragma once

#include <glm/vec2.hpp>

#include <optional>

namespace ge
{
    class Window;
    class Render;

    class RenderLoop final
    {
    public:
        RenderLoop(ge::Window& window, ge::Render& render);

        bool stopped() const;
        void handle_window_events();

    private:
        using ProjVec2 = glm::vec2;
        using ModelVec2 = glm::vec2;

        template <typename T>
        void handle_window_event(const T&);

        ge::Window& window_;
        ge::Render& render_;
        bool stopped_;

        std::optional<ProjVec2> prev_move_mouse_pos_;
    };
}
