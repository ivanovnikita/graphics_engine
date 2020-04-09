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
        .default_size = ge::Size{width, height}
        , .min_size = ge::Size{100, 100}
        , .max_size = std::nullopt
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
        ge::Vertex{{0.f, 0.5f}, {1.f, 0.f, 0.f}}
        , ge::Vertex{{-0.5f, -0.5f}, {0.f, 1.f, 0.f}}
        , ge::Vertex{{0.5f, -0.5f}, {0.f, 0.f, 1.f}}
    };
    const std::array<uint16_t, 3> indices
    {
        0, 1, 2
    };
    render.set_object_to_draw(vertices, indices);

    render.set_camera_pos({0.f, 0.f});
    render.set_camera_scale(1.f / 300.f);

    render.draw_frame();

    ge::RenderLoop render_loop(*window, render);
    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
