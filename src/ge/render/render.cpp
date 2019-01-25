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

    void Render::draw_frame()
    {
        impl_->draw_frame();
    }
}
