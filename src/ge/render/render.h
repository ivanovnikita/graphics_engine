#pragma once

#include "ge/render/vertex.h"

#include <span>
#include <memory>
#include <functional>

namespace vk
{
    class Instance;
    class SurfaceKHR;

    template <typename Type, typename Dispatch>
    class UniqueHandle;

    class DispatchLoaderStatic;

    using UniqueSurfaceKHR = UniqueHandle<SurfaceKHR, DispatchLoaderStatic>;
}

namespace ge
{
    using SurfaceCreator = vk::UniqueSurfaceKHR(const vk::Instance&);

    class Render final
    {
    public:
        Render
        (
            const std::function<SurfaceCreator>&
          , const uint16_t surface_width
          , const uint16_t surface_heigth
        );
        ~Render();
        void set_object_to_draw(const std::span<const Vertex>, const std::span<const uint16_t> indices);
        void draw_frame();
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height);

        glm::vec2 camera_pos() const;
        void set_camera_pos(const glm::vec2&);
        float camera_scale() const;
        void set_camera_scale(float scale);

    private:
        class RenderImpl;
        std::unique_ptr<RenderImpl> impl_;
    };
}
