#include "ge/render/render.h"
#include "ge/window/window.h"
#include "ge/render_loop/render_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <vulkan/vulkan.hpp>

#include <thread>
#include <fstream>
#include <regex>

int main()
{
#ifdef GE_DEBUG_LAYERS_ENABLED
    constexpr int override = 1;
    setenv("VK_LAYER_PATH", std::string{ge::VK_LAYER_PATH}.c_str(), override);
#endif

    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    constexpr ge::DynamicSize size
    {
        ge::Size{width, height}
        , ge::Size{100, 100}
        , ge::Size{700, 700}
    };
    auto window = ge::Window::create(size);

    ge::Render render
    (
        [&window] (const vk::Instance& instance)
        {
            return window->create_surface(instance);
        }
        , width
        , height
    );

    window->start_display();

    const std::array vertices
    {
        ge::Vertex{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}
        , ge::Vertex{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}
        , ge::Vertex{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    const std::array<uint16_t, 3> indices
    {
        0, 1, 2
    };
    render.set_object_to_draw(vertices, indices);

    render.draw_frame();

    ge::RenderLoop render_loop(*window, render);
    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}
