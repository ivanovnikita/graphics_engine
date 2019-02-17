#pragma once

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
        void draw_frame();
        void resize(const uint16_t new_surface_width, const uint16_t new_surface_height);

    private:
        class RenderImpl;
        std::unique_ptr<RenderImpl> impl_;
    };
}
