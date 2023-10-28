#include "camera_2d.h"

namespace ge
{
    namespace
    {
        glm::vec2 calc_ortho_proj
        (
            const uint32_t surface_width,
            const uint32_t surface_height,
            const float scale
        )
        {
            return
            {
                (1.f / static_cast<float>(surface_width)) / scale,
                (1.f / static_cast<float>(surface_height)) / scale
            };
        }
    }

    Camera2d::Camera2d
    (
        const glm::vec2 pos,
        const float scale,
        const uint32_t surface_width,
        const uint32_t surface_height
    )
        : transform_
        {
            .pos = pos,
            .ortho_proj = calc_ortho_proj(surface_width, surface_height, scale)
        }
        , scale_{scale}
        , surface_width_{surface_width}
        , surface_height_{surface_height}

    {
    }

    glm::vec2 Camera2d::get_pos() const noexcept
    {
        return transform_.pos;
    }

    float Camera2d::get_scale() const noexcept
    {
        return scale_;
    }

    const ViewProj2d& Camera2d::get_view_proj_2d() const noexcept
    {
        return transform_;
    }

    uint32_t Camera2d::get_surface_width() const noexcept
    {
        return surface_width_;
    }

    uint32_t Camera2d::get_surface_height() const noexcept
    {
        return surface_height_;
    }

    void Camera2d::set_pos(const glm::vec2 pos) noexcept
    {
        transform_.pos = pos;
    }

    void Camera2d::set_scale(const float scale) noexcept
    {
        scale_ = scale;
        transform_.ortho_proj = calc_ortho_proj(surface_width_, surface_height_, scale_);
    }

    void Camera2d::set_surface_sizes(const uint32_t width, const uint32_t height) noexcept
    {
        surface_width_ = width;
        surface_height_ = height;
        transform_.ortho_proj = calc_ortho_proj(surface_width_, surface_height_, scale_);
    }

    glm::vec2 Camera2d::normalize_in_proj_space(const glm::vec2& coord) const noexcept
    {
        glm::vec2 result{coord};

        glm::vec2 surface_center{surface_width_ / 2, surface_height_ / 2};

        result.x -= surface_center.x;
        result.x /= surface_center.x;

        result.y -= surface_center.y;
        result.y /= surface_center.y;

        return result;
    }

    glm::vec2 Camera2d::proj_to_model_space(const glm::vec2& coord) const noexcept
    {
        glm::vec2 result{coord.x, -coord.y};

        result.x /= transform_.ortho_proj.x;
        result.y /= transform_.ortho_proj.y;

        result += transform_.pos;

        return result;
    }
}
