module;

#include <glm/mat4x4.hpp>

export module camera_3d;

export import mvp_3d;
export import world_3d_coords;
export import extent;

namespace ge
{
    export class Camera3d final
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
        void set_surface_extent(Extent<uint32_t>);

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
