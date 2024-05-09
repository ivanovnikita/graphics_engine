#include "camera_3d.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ge
{
    namespace
    {
        glm::mat4 calc_projection
        (
            const Extent<uint32_t> surface_extent,
            const float projection_angle,
            const float projection_near_plane,
            const float projection_far_plane
        )
        {
            return glm::perspective
            (
                glm::radians(projection_angle),
                static_cast<float>(surface_extent.width) / static_cast<float>(surface_extent.height),
                projection_near_plane,
                projection_far_plane
            );
        }
    }

    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/geometry/how-does-matrix-work-part-1.html
    // https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/cam-nav-controls/3d-nav-cam-nav-controls.html
    // https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/

    // По умолчанию камера направлена в сторону -z. Почему?
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/computing-pixel-coordinates-of-3d-point/mathematics-computing-2d-coordinates-of-3d-points.html
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/projection-matrices-what-you-need-to-know-first.html
    // Кратко: это удобно для right-handed системы координат - для того, чтобы при проекции world-space -> screen space
    // точки по правую (левую) сторону оставались на правой (левой) стороне. Это достижимо только в случае
    // инвертирования z координаты. Фактически это инвертирование происходит в матрице проекции в коде glm.

    // Matrices are column-major, CS is right-handed
    // axis x: left-right
    // axis y: down-up
    // axis z: backward-forward
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
            calc_projection(surface_extent_, projection_angle_, projection_near_plane_, projection_far_plane_)
        }
        , pos_{glm::vec3{0.f}}
        , up_{0.f}
        , right_{0.f}
        , target_{0.f}
        , view_{1.f}
    {
    }

    void Camera3d::set_pos(const World3dCoords pos)
    {
        pos_ = pos;
        view_[3].x = pos_.coords.x;
        view_[3].y = pos_.coords.y;
        view_[3].z = pos_.coords.z;
    }

    World3dCoords Camera3d::get_pos() const noexcept
    {
        return pos_;
    }

    void Camera3d::set_rotate(const glm::mat3& m) noexcept
    {
        view_ = m;
    }

    void Camera3d::set_rotate(glm::vec3 up, glm::vec3 right, glm::vec3 target)
    {
        up_ = std::move(up);
        right_ = std::move(right);
        target_ = std::move(target);

        view_[0].x = right_.x;
        view_[0].y = right_.y;
        view_[0].z = right_.z;
        view_[1].x = up_.x;
        view_[1].y = up_.y;
        view_[1].z = up_.z;
        view_[2].x = target_.x;
        view_[2].y = target_.y;
        view_[2].z = target_.z;
    }

    const glm::vec3& Camera3d::get_rotate_up() const noexcept
    {
        return up_;
    }

    const glm::vec3& Camera3d::get_rotate_right() const noexcept
    {
        return right_;
    }

    const glm::vec3& Camera3d::get_rotate_target() const noexcept
    {
        return target_;
    }

    Mvp3d Camera3d::get_mvp(const ObjectTransform& model_transform) const
    {
        const glm::mat4 model_translate = glm::translate(model_transform.pos.coords);
        const glm::mat4 model_rotate
        {
            glm::normalize
            (
                glm::angleAxis(glm::radians(model_transform.rotate.z), glm::vec3{0.f, 0.f, 1.f}) *
                glm::angleAxis(glm::radians(model_transform.rotate.y), glm::vec3{0.f, 1.f, 0.f}) *
                glm::angleAxis(glm::radians(model_transform.rotate.x), glm::vec3{1.f, 0.f, 0.f})
            )
        };
        const glm::mat4 model_scale = glm::scale(model_transform.scale);
        const glm::mat4 model = model_translate * model_rotate * model_scale;

        // Камера на позиции {x,y,z} означает, что весь мир нужно сдвигать на дельту {-x,-y,-z}
        // Аналогично с вращением.
        const glm::mat4 view = glm::inverse(view_);

        return Mvp3d{projection_ * view * model};
    }

    void Camera3d::set_surface_extent(const Extent<uint32_t> extent)
    {
        surface_extent_ = extent;
        projection_ = calc_projection(surface_extent_, projection_angle_, projection_near_plane_, projection_far_plane_);
    }

    SurfaceNormalizedCoords Camera3d::normalize(const SurfaceCoords& coord) const noexcept
    {
        SurfaceNormalizedCoords result{coord.coords};

        glm::vec2 surface_center{surface_extent_.width / 2, surface_extent_.height / 2};

        result.coords.x -= surface_center.x;
        result.coords.x /= surface_center.x;

        result.coords.y -= surface_center.y;
        result.coords.y /= surface_center.y;

        return result;
    }
}
