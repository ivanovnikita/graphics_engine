module;

#include <span>

export module camera_2d;

export import extent;
export import surface_normalized_coords;
export import world_2d_coords;
export import surface_coords;
export import view_proj_2d;

namespace ge
{
    export class Camera2d final
    {
    public:
        explicit Camera2d
        (
            World2dCoords pos,
            float scale,
            Extent<uint32_t> surface_extent
        ) noexcept;

        World2dCoords get_pos() const noexcept;
        float get_scale() const noexcept;
        const ViewProj2d& get_view_proj_2d() const noexcept;
        Extent<uint32_t> get_surface_extent() const noexcept;

        void set_pos(World2dCoords) noexcept;
        void set_scale(float) noexcept;
        void set_surface_extent(Extent<uint32_t>) noexcept;

        SurfaceNormalizedCoords normalize(const SurfaceCoords&) const noexcept;
        World2dCoords to_world_space(const SurfaceNormalizedCoords&) const noexcept;

        void camera_on_center(const std::span<const World2dCoords>&) noexcept;
        void scale_to_fit_all(const std::span<const World2dCoords>&) noexcept;

    private:
        ViewProj2d transform_;
        float scale_;
        Extent<uint32_t> surface_extent_;
    };
}
