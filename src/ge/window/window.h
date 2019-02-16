#pragma once

#include "ge/window/window_events.h"

#include <memory>
#include <vector>
#include <variant>
#include <optional>

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
    struct Size final
    {
        uint16_t width;
        uint16_t height;
    };

    using StaticSize = Size;

    struct DynamicSize final
    {
        Size default_size;
        std::optional<Size> min_size;
        std::optional<Size> max_size;
    };

    using WindowSize = std::variant<StaticSize, DynamicSize>;

    class Window
    {
    public:
        virtual ~Window();

        static std::unique_ptr<Window> create(const WindowSize&);

        virtual vk::UniqueSurfaceKHR create_surface(const vk::Instance& instance) = 0;

        virtual void start_display() = 0;
        virtual std::vector<WindowEvent> grab_events() = 0;

    protected:
        Window();
    };
}
