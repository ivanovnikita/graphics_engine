#include "ge/render/2d_graph/render_2d_graph.h"
#include "ge/common/exception.h"
#include "ge/window/linux/window_xcb.h"
#include "ge/render_loop/render_2d_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <iostream>
#include <fstream>
#include <thread>

using Cl = ge::Color;

constexpr Cl BL{{0.f, 0.f, 0.f}};
constexpr Cl GR{{0.5f, 0.5f, 0.5f}};

namespace
{
    std::vector<std::vector<ge::World2dCoords>> read_points(const std::string_view& filepath)
    {
        std::ifstream file;
        file.open(std::string{filepath}, std::ios::binary | std::ios::in);

        std::vector<std::vector<ge::World2dCoords>> result;

        while (not file.eof() and not file.fail())
        {
            uint64_t size = 0;
            file.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));

            if (size == 0)
            {
                continue;
            }

            result.emplace_back(std::vector<ge::World2dCoords>{});
            for (size_t i = 0; i < size; i += 2)
            {
                double x = 0;
                double y = 0;

                file.read(reinterpret_cast<char*>(&x), sizeof(double));
                file.read(reinterpret_cast<char*>(&y), sizeof(double));

                result.back().emplace_back(ge::World2dCoords{glm::vec2{y, x}});
            }
        }

        file.close();

        return result;
    }

    std::vector<ge::graph::Graph::Vertice> init_vertices(const std::vector<ge::World2dCoords>& points)
    {
        std::vector<ge::graph::Graph::Vertice> result;

        for (size_t i = 0; i < points.size(); ++i)
        {
            result.emplace_back(ge::graph::Graph::Vertice{i, BL});
        }

        return result;
    }

    std::pair<std::vector<ge::World2dCoords>, std::vector<ge::graph::Graph::Arc>> init_arcs
    (
        const std::vector<std::vector<ge::World2dCoords>>& raw
    )
    {
        std::vector<ge::World2dCoords> points;
        std::vector<ge::graph::Graph::Arc> arcs;

        for (const std::vector<ge::World2dCoords>& road : raw)
        {
            if (road.empty())
            {
                continue;
            }

            points.emplace_back(road.front());
            for (size_t i = 1; i < road.size(); ++i)
            {
                points.emplace_back(road[i]);
                arcs.emplace_back(ge::graph::Graph::Arc{points.size() - 2, points.size() - 1, GR});
            }
        }

        return {std::move(points), std::move(arcs)};
    }

    namespace triangle
    {
        constexpr std::array points
        {
            ge::World2dCoords{{0.f, 0.5f}}
            , ge::World2dCoords{{-0.5f, -0.5f}}
            , ge::World2dCoords{{0.5f, -0.5f}}
        };
        constexpr std::array vertices
        {
            ge::graph::Graph::Vertice{.index = 0, .color = ge::Color{{1.f, 0.f, 0.f}}}
          , ge::graph::Graph::Vertice{.index = 1, .color = ge::Color{{0.f, 1.f, 0.f}}}
          , ge::graph::Graph::Vertice{.index = 2, .color = ge::Color{{0.f, 0.f, 1.f}}}
        };
        constexpr std::array arcs
        {
            ge::graph::Graph::Arc{.index_from = 0, .index_to = 1, .color = ge::Color{{1.f, 0.f, 0.f}}}
          , ge::graph::Graph::Arc{.index_from = 1, .index_to = 2, .color = ge::Color{{0.f, 1.f, 0.f}}}
          , ge::graph::Graph::Arc{.index_from = 2, .index_to = 0, .color = ge::Color{{0.f, 0.f, 1.f}}}
        };
    }
}

int main(int argc, char* argv[])
{
    using namespace ge;
    using namespace ge::graph;

#ifdef GE_DEBUG_LAYERS_ENABLED
    constexpr int override = 1;
    setenv("VK_LAYER_PATH", std::string{ge::VK_LAYER_PATH}.c_str(), override);
#endif

    try
    {
        const Logger logger
        {
            Flags<LogType>
            {
                LogType::Error,
                LogType::ErrorDetails,
//                LogType::SystemInfo
            }
        };

        constexpr uint16_t width = 1024;
        constexpr uint16_t height = 768;
        constexpr DynamicSize size
        {
            .default_size = Size{width, height}
            , .min_size = Size{100, 100}
            , .max_size = std::nullopt
        };
        constexpr std::array<uint8_t, 4> background_color{255, 255, 255, 1};
        auto window = WindowXCB(size, background_color, logger);

        Render2dGraph render
        (
            ge::SurfaceParams
            {
                .surface = XcbSurface{window.get_connection(), window.get_window()}
                , .width = width
                , .height = height
                , .background_color = background_color
            },
            logger
        );

        window.start_display();

        if (argc == 1)
        {
            const Graph graph
            {
                triangle::points
              , triangle::vertices
              , triangle::arcs
            };

            render.set_object_to_draw(graph);

            Camera2d camera = render.get_camera();
            camera.camera_on_center(triangle::points);
            camera.set_scale(1.f / 300.f);
            render.set_camera(std::move(camera));
        }
        else
        {
            std::string_view graph_file_path{argv[1]};
            const std::vector<std::vector<World2dCoords>> raw_points = read_points(graph_file_path);
            auto [points, arcs] = init_arcs(raw_points);

            const std::vector<Graph::Vertice> vertices = init_vertices(points);

            const Graph graph
            {
                points,
                vertices,
//                {},
                arcs
            };

            render.set_object_to_draw(graph);

            Camera2d camera = render.get_camera();
            camera.camera_on_center(points);
            camera.scale_to_fit_all(points);
            render.set_camera(std::move(camera));
        }

        render.draw_frame();

        Render2dLoop render_loop(window, render);
        while (not render_loop.stopped())
        {
            render_loop.handle_window_events();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const ge::expected_error& e)
    {
        std::cerr
            << "Expected error: [" << e.get_function_name()
            << "] [" << e.get_line_number()
            << "]: " << e.get_error_message()
            << " " << e.get_error_details() << std::endl;
    }
    catch (const ge::unexpected_error& e)
    {
        std::cerr
            << "Unexpected error: [" << e.get_function_name()
            << "] [" << e.get_line_number()
            << "]: " << e.get_error_message()
            << " " << e.get_error_details() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unexpected exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unexpected exception" << std::endl;
    }

    return 0;
} 
