#pragma once

#include <glm/vec2.hpp>

namespace ge
{
    struct ViewProj2d final
    {
        glm::vec2 pos;
        glm::vec2 ortho_proj; // https://jsantell.com/3d-projection/#orthographic-projection
    };

    class Camera2d final
    {
    public:
        explicit Camera2d
        (
            glm::vec2 pos,
            float scale,
            uint32_t surface_width,
            uint32_t surface_height
        );

        glm::vec2 get_pos() const noexcept;
        float get_scale() const noexcept;
        const ViewProj2d& get_view_proj_2d() const noexcept;
        uint32_t get_surface_width() const noexcept;
        uint32_t get_surface_height() const noexcept;

        void set_pos(glm::vec2 pos) noexcept;
        void set_scale(float) noexcept;
        void set_surface_sizes(uint32_t width, uint32_t height) noexcept;

        glm::vec2 normalize_in_proj_space(const glm::vec2&) const noexcept;
        glm::vec2 proj_to_model_space(const glm::vec2&) const noexcept;

    private:
        ViewProj2d transform_;
        float scale_;
        uint32_t surface_width_;
        uint32_t surface_height_;
    };

    // TODO implement Camera2dMover
    /*
    class Camera2dMover final
    {
    public:
        void zoom_in(speed);
        void zoom_out(speed);
        void move(direction, speed);
        void drag_move_start(drag_point);
        void drag_move(drag_point);
        void drag_move_end();
    }
    */
}
