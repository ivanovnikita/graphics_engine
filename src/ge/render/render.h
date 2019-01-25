#pragma once

#include <memory>
#include <functional>

namespace vk
{
    class Instance;
    class SurfaceKHR;

    template <typename Type>
    class UniqueHandle;

    using UniqueSurfaceKHR = UniqueHandle<SurfaceKHR>;
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

    private:
        class RenderImpl;
        std::unique_ptr<RenderImpl> impl_;
    };
}
