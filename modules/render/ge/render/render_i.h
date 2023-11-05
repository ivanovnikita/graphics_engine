#pragma once

#include "camera/camera_2d.h"

#include <cstdint>

namespace ge
{
    class DrawableI
    {
    public:
        virtual ~DrawableI();

        virtual void draw_frame() = 0;
        virtual void resize(Extent<uint32_t> new_surface_extent) = 0;
    };

    class WithCamera2dI
    {
    public:
        virtual ~WithCamera2dI();

        virtual const Camera2d& get_camera() const = 0;
        virtual void set_camera(Camera2d) = 0;
    };

    class Render2dI : public virtual DrawableI, public WithCamera2dI
    {
    public:
        virtual ~Render2dI();
    };
}
