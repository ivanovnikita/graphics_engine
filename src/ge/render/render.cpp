#include "ge/render/render.h"
#include "ge/render/render_impl.h"

#include <vulkan/vulkan.h>

namespace ge
{
    Render::Render
    (
        const std::function<SurfaceCreator>& create_surface
      , const uint16_t surface_width
      , const uint16_t surface_heigth
    )
        : impl_
        (
            new RenderImpl
            (
                create_surface
              , vk::Extent2D{}
                  .setWidth(surface_width)
                  .setHeight(surface_heigth)
            )
        )
    {
    }

    Render::~Render() = default;

    void Render::set_object_to_draw(const std::span<const Vertex> vertices, const std::span<const uint16_t> indices)
    {
        impl_->set_object_to_draw(vertices, indices);
    }

    void Render::draw_frame()
    {
        impl_->draw_frame();
    }

    void Render::resize(const uint16_t new_surface_width, const uint16_t new_surface_height)
    {
        impl_->resize(new_surface_width, new_surface_height);
    }

    glm::vec2 Render::camera_pos() const
    {
        return impl_->camera_pos();
    }

    void Render::set_camera_pos(const glm::vec2& pos)
    {
        impl_->set_camera_pos(pos);
    }

    float Render::camera_scale() const
    {
        return impl_->camera_scale();
    }

    void Render::set_camera_scale(float scale)
    {
        impl_->set_camera_scale(scale);
    }
}
