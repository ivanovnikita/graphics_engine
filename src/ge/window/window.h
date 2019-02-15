#pragma once

#include "ge/window/window_events.h"

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

        static std::unique_ptr<Window> create(uint16_t width, uint16_t height);

        virtual vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) = 0;
        virtual vk::Extent2D extent() const = 0;

        virtual void start_display() = 0;
        virtual std::vector<WindowEvent> grab_events() = 0;

    protected:
        Window();
    };
}
