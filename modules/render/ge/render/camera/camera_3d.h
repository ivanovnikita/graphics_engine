#pragma once

#include "mvp_3d.h"

#include "ge/render/extent.h"

#include "ge/geometry/world_3d_coords.h"

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

        Mvp3d get_mvp(const World3dCoords& model_coords) const;

        void set_pos(World3dCoords);

    private:
        Extent<uint32_t> surface_extent_;
        float projection_angle_;
        float projection_near_plane_;
        float projection_far_plane_;
        glm::mat4 projection_;

        World3dCoords pos_;
        glm::mat4 view_;
    };
}
