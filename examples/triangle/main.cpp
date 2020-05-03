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
    constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};

    auto window = ge::Window::create(size, background_color);

    ge::Render render
    (
        ge::SurfaceParams
        {
            .surface_creator = [&window] (const vk::Instance& instance)
            {
                return window->create_surface(instance);
            }
            , .width = width
            , .height = height
            , .background_color = background_color
        }
    );

    window->start_display();

    const std::array points
    {
        ge::Vertex{{0.f, 0.5f}}
        , ge::Vertex{{-0.5f, -0.5f}}
        , ge::Vertex{{0.5f, -0.5f}}
    };
    const std::array vertices
    {
        ge::Graph::Vertice{.index = 0, .color = ge::Color{{1.f, 0.f, 0.f}}}
      , ge::Graph::Vertice{.index = 1, .color = ge::Color{{0.f, 1.f, 0.f}}}
      , ge::Graph::Vertice{.index = 2, .color = ge::Color{{0.f, 0.f, 1.f}}}
    };
    const std::array arcs
    {
        ge::Graph::Arc{.index_from = 0, .index_to = 1, .color = ge::Color{{1.f, 0.f, 0.f}}}
      , ge::Graph::Arc{.index_from = 1, .index_to = 2, .color = ge::Color{{0.f, 1.f, 0.f}}}
      , ge::Graph::Arc{.index_from = 2, .index_to = 0, .color = ge::Color{{0.f, 0.f, 1.f}}}
    };
    const ge::Graph graph
    {
        points
      , vertices
      , arcs
    };
    render.set_object_to_draw(graph);

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
