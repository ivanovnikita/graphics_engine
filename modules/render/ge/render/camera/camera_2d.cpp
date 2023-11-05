#include "camera_2d.h"

namespace ge
{
    namespace
    {
        glm::vec2 calc_ortho_proj
        (
            const Extent<uint32_t> surface_extent,
            const float scale
        )
        {
            return
            {
                (1.f / static_cast<float>(surface_extent.width)) / scale,
                (1.f / static_cast<float>(surface_extent.height)) / scale
            };
        }
    }

    Camera2d::Camera2d
    (
        const World2dCoords pos,
        const float scale,
        const Extent<uint32_t> surface_extent
    ) noexcept
        : transform_
        {
            .camera_pos = pos,
            .ortho_proj = calc_ortho_proj(surface_extent, scale)
        }
        , scale_{scale}
        , surface_extent_{surface_extent}

    {
    }

    World2dCoords Camera2d::get_pos() const noexcept
    {
        return transform_.camera_pos;
    }

    float Camera2d::get_scale() const noexcept
    {
        return scale_;
    }

    const ViewProj2d& Camera2d::get_view_proj_2d() const noexcept
    {
        return transform_;
    }

    Extent<uint32_t> Camera2d::get_surface_extent() const noexcept
    {
        return surface_extent_;
    }

    void Camera2d::set_pos(const World2dCoords pos) noexcept
    {
        transform_.camera_pos = pos;
    }

    void Camera2d::set_scale(const float scale) noexcept
    {
        scale_ = scale;
        transform_.ortho_proj = calc_ortho_proj(surface_extent_, scale_);
    }

    void Camera2d::set_surface_extent(const Extent<uint32_t> extent) noexcept
    {
        surface_extent_ = extent;
        transform_.ortho_proj = calc_ortho_proj(surface_extent_, scale_);
    }

    SurfaceNormalizedCoords Camera2d::normalize(const SurfaceCoords& coord) const noexcept
    {
        SurfaceNormalizedCoords result{coord.coords};

        glm::vec2 surface_center{surface_extent_.width / 2, surface_extent_.height / 2};

        result.coords.x -= surface_center.x;
        result.coords.x /= surface_center.x;

        result.coords.y -= surface_center.y;
        result.coords.y /= surface_center.y;

        return result;
    }

    World2dCoords Camera2d::to_world_space(const SurfaceNormalizedCoords& coord) const noexcept
    {
        World2dCoords result{.coords = {coord.coords.x, -coord.coords.y}};

        result.coords.x /= transform_.ortho_proj.x;
        result.coords.y /= transform_.ortho_proj.y;

        result.coords += transform_.camera_pos.coords;

        return result;
    }

    void Camera2d::camera_on_center(const std::span<const World2dCoords>& points) noexcept
    {
        float min_x = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();

        float min_y = std::numeric_limits<float>::max();
        float max_y = std::numeric_limits<float>::min();

        for (const World2dCoords& point : points)
        {
            min_x = std::min(min_x, point.coords.x);
            max_x = std::max(max_x, point.coords.x);

            min_y = std::min(min_y, point.coords.y);
            max_y = std::max(max_y, point.coords.y);
        }

        set_pos
        (
            World2dCoords
            {
                glm::vec2
                {
                    (min_x + max_x) / 2.f,
                    (min_y + max_y) / 2.f
                }
            }
        );
    }

    void Camera2d::scale_to_fit_all(const std::span<const World2dCoords>& points) noexcept
    {
        float min_x = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();

        float min_y = std::numeric_limits<float>::max();
        float max_y = std::numeric_limits<float>::min();

        for (const World2dCoords& point : points)
        {
            min_x = std::min(min_x, point.coords.x);
            max_x = std::max(max_x, point.coords.x);

            min_y = std::min(min_y, point.coords.y);
            max_y = std::max(max_y, point.coords.y);
        }

        const float width_in_model_space = max_x - min_x;
        const float height_in_model_space = max_y - min_y;

        const float width_scale = width_in_model_space / static_cast<float>(surface_extent_.width);
        const float height_scale = height_in_model_space / static_cast<float>(surface_extent_.height);

        set_scale(std::min(width_scale, height_scale));
    }
}
