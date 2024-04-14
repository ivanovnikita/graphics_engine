module;

//#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
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

module : private;

namespace ge
{
    Camera3d::Camera3d
    (
        const Extent<uint32_t> surface_extent,
        const float projection_angle,
        const float projection_near_plane,
        const float projection_far_plane
    )
        : surface_extent_{surface_extent}
        , projection_angle_{projection_angle}
        , projection_near_plane_{projection_near_plane}
        , projection_far_plane_{projection_far_plane}
        , projection_
        {
            glm::perspective
            (
                glm::radians(projection_angle_),
                static_cast<float>(surface_extent_.width) / static_cast<float>(surface_extent_.height),
                projection_near_plane_,
                projection_far_plane_
            )
        }
    {
    }

    Mvp3d Camera3d::get_mvp(const World3dCoords& model_coords) const
    {
        const glm::mat4 model = glm::translate(model_coords.coords);
        return Mvp3d{projection_ * view_ * model};
    }

    void Camera3d::set_pos(const World3dCoords pos)
    {
        pos_ = pos;
//        view_ = glm::translate(pos_.coords);
        view_ = glm::lookAt
        (
            pos_.coords,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
        /*
        glm::mat4 CameraMatrix = glm::lookAt
        (
            cameraPosition, // the position of your camera, in world space
            cameraTarget,   // where you want to look at, in world space
            upVector        // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
        );
        */
    }

    void Camera3d::set_surface_extent(const Extent<uint32_t> extent)
    {
        surface_extent_ = extent;
        projection_ = glm::mat4
        {
            glm::perspective
            (
                glm::radians(projection_angle_),
                static_cast<float>(surface_extent_.width) / static_cast<float>(surface_extent_.height),
                projection_near_plane_,
                projection_far_plane_
            )
        };
    }
}
