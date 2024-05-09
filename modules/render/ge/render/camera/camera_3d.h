#pragma once

#include "mvp_3d.h"

#include "ge/render/extent.h"
#include "ge/render/object_transform.h"

#include "ge/geometry/world_3d_coords.h"
#include "ge/geometry/surface_normalized_coords.h"

#include "ge/window/surface_coords.h"

namespace ge
{
    class Camera3d final
    {
    public:
        explicit Camera3d
        (
            Extent<uint32_t> surface_extent,
            float projection_angle,
            float projection_near_plane,
            float projection_far_plane
        );

        Mvp3d get_mvp(const ObjectTransform& model_transform) const;

        World3dCoords get_pos() const noexcept;
        void set_pos(World3dCoords);

        void set_rotate(glm::vec3 up, glm::vec3 right, glm::vec3 target);
        const glm::vec3& get_rotate_up() const noexcept;
        const glm::vec3& get_rotate_right() const noexcept;
        const glm::vec3& get_rotate_target() const noexcept;
        void set_rotate(const glm::mat3&) noexcept;

        void set_surface_extent(Extent<uint32_t>);

        SurfaceNormalizedCoords normalize(const SurfaceCoords&) const noexcept;

    private:
        Extent<uint32_t> surface_extent_;
        float projection_angle_;
        float projection_near_plane_;
        float projection_far_plane_;
        glm::mat4 projection_;

        World3dCoords pos_;
        glm::vec3 up_;
        glm::vec3 right_;
        glm::vec3 target_;

        glm::mat4 view_;
    };
}
