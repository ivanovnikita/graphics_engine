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
#include <span>

namespace grid
{
/*

            1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1,
            1, 0, 0, 1, 0, 0, 1,
            1, 0, 0, 1, 0, 0, 1,
            1, 1, 1, 1, 1, 1, 1

*/
    using C = ge::Vertex;

    constexpr std::array points
    {
        C{{0.f, 0.f}}, C{{0.f, 1.f}}, C{{0.f, 2.f}}, C{{0.f, 3.f}}, C{{0.f, 4.f}}, C{{0.f, 5.f}}, C{{0.f, 6.f}},
        C{{1.f, 0.f}}, C{{1.f, 1.f}}, C{{1.f, 2.f}}, C{{1.f, 3.f}}, C{{1.f, 4.f}}, C{{1.f, 5.f}}, C{{1.f, 6.f}},
        C{{2.f, 0.f}}, C{{2.f, 1.f}}, C{{2.f, 2.f}}, C{{2.f, 3.f}}, C{{2.f, 4.f}}, C{{2.f, 5.f}}, C{{2.f, 6.f}},
        C{{3.f, 0.f}}, C{{3.f, 1.f}}, C{{3.f, 2.f}}, C{{3.f, 3.f}}, C{{3.f, 4.f}}, C{{3.f, 5.f}}, C{{3.f, 6.f}},
        C{{4.f, 0.f}}, C{{4.f, 1.f}}, C{{4.f, 2.f}}, C{{4.f, 3.f}}, C{{4.f, 4.f}}, C{{4.f, 5.f}}, C{{4.f, 6.f}},
        C{{5.f, 0.f}}, C{{5.f, 1.f}}, C{{5.f, 2.f}}, C{{5.f, 3.f}}, C{{5.f, 4.f}}, C{{5.f, 5.f}}, C{{5.f, 6.f}},
        C{{6.f, 0.f}}, C{{6.f, 1.f}}, C{{6.f, 2.f}}, C{{6.f, 3.f}}, C{{6.f, 4.f}}, C{{6.f, 5.f}}, C{{6.f, 6.f}}
    };

    using V = ge::Graph::Vertice;
    using Cl = ge::Color;

    constexpr Cl BL{{0.f, 0.f, 0.f}};
    constexpr Cl WH{{1.f, 1.f, 1.f}};

    constexpr std::array vertices
    {
        V{0, WH}, V{1, WH}, V{2, WH}, V{3, WH}, V{4, WH}, V{5, WH}, V{6, WH},
        V{7, WH}, V{8, BL}, V{9, BL}, V{10, WH}, V{11, BL}, V{12, BL}, V{13, WH},
        V{14, WH}, V{15, BL}, V{16, BL}, V{17, WH}, V{18, BL}, V{19, BL}, V{20, WH},
        V{21, WH}, V{22, WH}, V{23, WH}, V{24, WH}, V{25, WH}, V{26, WH}, V{27, WH},
        V{28, WH}, V{29, BL}, V{30, BL}, V{31, WH}, V{32, BL}, V{33, BL}, V{34, WH},
        V{35, WH}, V{36, BL}, V{37, BL}, V{38, WH}, V{39, BL}, V{40, BL}, V{41, WH},
        V{42, WH}, V{43, WH}, V{44, WH}, V{45, WH}, V{46, WH}, V{47, WH}, V{48, WH},
    };
    constexpr std::array<ge::Graph::Arc, 0> arcs
    {
    };
}

namespace
{
    glm::vec2 camera_on_center(const std::span<const ge::Vertex>& points)
    {
        float min_x = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();

        float min_y = std::numeric_limits<float>::max();
        float max_y = std::numeric_limits<float>::min();

        for (const ge::Vertex& point : points)
        {
            min_x = std::min(min_x, point.pos.x);
            max_x = std::max(max_x, point.pos.x);

            min_y = std::min(min_y, point.pos.y);
            max_y = std::max(max_y, point.pos.y);
        }

        return glm::vec2
        {
            (min_x + max_x) / 2.f
          , (min_y + max_y) / 2.f
        };
    }
}

int main(int argc, char* /*argv*/[])
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
//    constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};
    constexpr std::array<uint8_t, 4> background_color{100, 100, 100, 1};

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

    if (argc == 1)
    {
        const ge::Graph graph
        {
            grid::points
          , grid::vertices
          , grid::arcs
        };

        render.set_object_to_draw(graph);

        const glm::vec2 camera_pos = camera_on_center(grid::points);
        render.set_camera_pos(camera_pos);
    }

//    render.set_camera_scale(1.f / 300.f);
    render.set_camera_scale(1.f / 27.f);

    render.draw_frame();

    ge::RenderLoop render_loop(*window, render);
    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
