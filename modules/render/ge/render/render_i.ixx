module;

#include <cstdint>

export module render_i;

export import camera_2d;
export import camera_3d;
export import extent;

namespace ge
{
    export class DrawableI
    {
    public:
        virtual ~DrawableI();

        virtual void draw_frame() = 0;
        virtual void resize(Extent<uint32_t> new_surface_extent) = 0;
    };

    export class WithCamera2dI
    {
    public:
        virtual ~WithCamera2dI();

        virtual const Camera2d& get_camera() const = 0;
        virtual void set_camera(Camera2d) = 0;
    };

    export class WithCamera3dI
    {
    public:
        virtual ~WithCamera3dI();

        virtual const Camera3d& get_camera() const = 0;
        virtual void set_camera(Camera3d) = 0;
    };

    export class Render2dI : public virtual DrawableI, public WithCamera2dI
    {
    public:
        virtual ~Render2dI() override;
    };

    export class Render3dI : public virtual DrawableI, public WithCamera3dI
    {
    public:
        virtual ~Render3dI() override;
    };
}
