module;

#include <glm/glm.hpp>

#include <functional>
#include <optional>

export module camera_2d_mover;

export import render_i;

namespace ge
{
    export enum class ZoomDirection
    {
        In,
        Out
    };

    export class Camera2dMover final
    {
    public:
        explicit Camera2dMover(WithCamera2dI&);

        void zoom(const SurfaceCoords& zoom_point, ZoomDirection);

        void drag_move_start(const SurfaceCoords& drag_point);
        void drag_move(const SurfaceCoords& drag_point);
        void drag_move_end();

        // TODO
        // void move(direction, speed);

    private:
        std::reference_wrapper<WithCamera2dI> render_;
        std::optional<World2dCoords> last_drag_point_;
    };
}
