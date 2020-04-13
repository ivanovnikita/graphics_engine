#pragma once

#include "ge/window/window_events.h"
#include "ge/window/window_size.h"

#include <memory>
#include <vector>

namespace vk
{
    class Instance;
    class SurfaceKHR;
    struct Extent2D;

    template <typename Type, typename Dispatch>
    class UniqueHandle;

    class DispatchLoaderStatic;

    using UniqueSurfaceKHR = UniqueHandle<SurfaceKHR, DispatchLoaderStatic>;
}

namespace ge
{
    class Window
    {
    public:
        virtual ~Window();

        static std::unique_ptr<Window> create
        (
            const WindowSize&
            , const std::array<uint8_t, 4> background_color
        );

        virtual vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) = 0;

        virtual void start_display() = 0;
        virtual std::vector<WindowEvent> grab_events() = 0;
        virtual void resize(const Size&) = 0;

    protected:
        Window();
    };
}
