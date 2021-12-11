#pragma once

#include "ge/render/render_i.h"

#include <glm/glm.hpp>

#include <functional>
#include <optional>

namespace ge
{
    enum class ZoomDirection
    {
        In,
        Out
    };

    class Camera2dMover final
    {
    public:
        explicit Camera2dMover(RenderI&);

        void zoom(const glm::vec2& zoom_point, ZoomDirection);

        void drag_move_start(const glm::vec2& drag_point);
        void drag_move(const glm::vec2& drag_point);
        void drag_move_end();

        // TODO
        // void move(direction, speed);

    private:
        std::reference_wrapper<RenderI> render_;
        std::optional<glm::vec2> last_drag_point_;
    };


}
