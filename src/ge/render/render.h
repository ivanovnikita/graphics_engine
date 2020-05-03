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

    struct SurfaceParams final
    {
        std::function<SurfaceCreator> surface_creator;
        uint32_t width;
        uint32_t height;
        std::array<uint8_t, 4> background_color;
    };

    class Render final
    {
    public:
        Render
        (
            const SurfaceParams&
        );
        ~Render();

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
