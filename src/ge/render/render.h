#pragma once

#include "ge/render/2d_graph/2d_graph.h"
#include "ge/render/vertex.h"
#include "ge/render/surface_params.h"

#include <span>
#include <memory>

namespace ge
{
    enum class DrawMode
    {
        POLYGONS,
        GRAPH
    };

    class Render final
    {
    public:
        Render
        (
            const SurfaceParams&,
            DrawMode
        );
        ~Render();

        void set_object_to_draw(const std::span<const Polygons>&);
        void set_object_to_draw(const Graph&);
        void draw_frame();
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height);

        glm::vec2 camera_pos() const;
        void set_camera_pos(const glm::vec2&);
        float camera_scale() const;
        void set_camera_scale(float scale);

        glm::vec2 normalize_in_proj_space(const glm::vec2&) const;
        glm::vec2 proj_to_model_space(const glm::vec2&) const;

    private:
        class RenderImpl;
        std::unique_ptr<RenderImpl> impl_;
    };
}
