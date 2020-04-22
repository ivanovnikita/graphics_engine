#include "ge/render/render.h"
#include "ge/render/render_impl.h"

#include <vulkan/vulkan.h>

namespace ge
{
    Render::Render
    (
        const SurfaceParams& surface_params
      , const VerticesInterpretation& vertices_interpretation
    )
        : impl_
        (
            new RenderImpl
            (
                surface_params
              , vertices_interpretation
            )
        )
    {
    }

    Render::~Render() = default;

    void Render::set_object_to_draw
    (
        const std::span<const Vertex> vertices
        , const std::span<const Color> colors
        , const std::span<const uint16_t> indices
    )
    {
        impl_->set_object_to_draw(vertices, colors, indices);
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

    glm::vec2 Render::normalize_in_proj_space(const glm::vec2& coord) const
    {
        return impl_->normalize_in_proj_space(coord);
    }

    glm::vec2 Render::proj_to_model_space(const glm::vec2& coord) const
    {
        return impl_->proj_to_model_space(coord);
    }
}
