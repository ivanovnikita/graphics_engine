#include "ge/render/2d_graph/render_2d_graph.h"
#include "ge/render/exception.h"
#include "ge/window/linux/window_xcb.h"
#include "ge/render_loop/render_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <vulkan/vulkan.hpp>

#include <thread>
#include <fstream>
#include <regex>
#include <span>
#include <iostream>

using Cl = ge::Color;

//constexpr Cl BL{{0.f, 0.f, 0.f}};
//constexpr Cl GR{{0.5f, 0.5f, 0.5f}};

namespace
{
//    std::vector<std::vector<ge::Vertex>> read_points(const std::string_view& filepath)
//    {
//        std::ifstream file;
//        file.open(std::string{filepath}, std::ios::binary | std::ios::in);

//        std::vector<std::vector<ge::Vertex>> result;

//        while (not file.eof() and not file.fail())
//        {
//            uint64_t size = 0;
//            file.read(reinterpret_cast<char*>(&size), sizeof(uint64_t));

//            if (size == 0)
//            {
//                continue;
//            }

//            result.emplace_back(std::vector<ge::Vertex>{});
//            for (size_t i = 0; i < size; i += 2)
//            {
//                double x = 0;
//                double y = 0;

//                file.read(reinterpret_cast<char*>(&x), sizeof(double));
//                file.read(reinterpret_cast<char*>(&y), sizeof(double));

//                result.back().emplace_back(ge::Vertex{glm::vec2{y, x}});
//            }
//        }

//        file.close();

//        return result;
//    }

//    std::vector<ge::Graph::Vertice> init_vertices(const std::vector<ge::Vertex>& points)
//    {
//        std::vector<ge::Graph::Vertice> result;

//        for (size_t i = 0; i < points.size(); ++i)
//        {
//            result.emplace_back(ge::Graph::Vertice{i, BL});
//        }

//        return result;
//    }

//    std::pair<std::vector<ge::Vertex>, std::vector<ge::Graph::Arc>> init_arcs
//    (
//        const std::vector<std::vector<ge::Vertex>>& raw
//    )
//    {
//        std::vector<ge::Vertex> points;
//        std::vector<ge::Graph::Arc> arcs;

//        for (const std::vector<ge::Vertex>& road : raw)
//        {
//            if (road.empty())
//            {
//                continue;
//            }

//            points.emplace_back(road.front());
//            for (size_t i = 1; i < road.size(); ++i)
//            {
//                points.emplace_back(road[i]);
//                arcs.emplace_back(ge::Graph::Arc{points.size() - 2, points.size() - 1, GR});
//            }
//        }

//        return {std::move(points), std::move(arcs)};
//    }

//    glm::vec2 camera_on_center(const std::span<const ge::Vertex>& points)
//    {
//        float min_x = std::numeric_limits<float>::max();
//        float max_x = std::numeric_limits<float>::min();

//        float min_y = std::numeric_limits<float>::max();
//        float max_y = std::numeric_limits<float>::min();

//        for (const ge::Vertex& point : points)
//        {
//            min_x = std::min(min_x, point.pos.x);
//            max_x = std::max(max_x, point.pos.x);

//            min_y = std::min(min_y, point.pos.y);
//            max_y = std::max(max_y, point.pos.y);
//        }

//        return glm::vec2
//        {
//            (min_x + max_x) / 2.f
//          , (min_y + max_y) / 2.f
//        };
//    }

//    float scale_to_fit_all
//    (
//        const std::span<const ge::Vertex>& points,
//        const uint16_t window_width,
//        const uint16_t window_height
//    )
//    {
//        float min_x = std::numeric_limits<float>::max();
//        float max_x = std::numeric_limits<float>::min();

//        float min_y = std::numeric_limits<float>::max();
//        float max_y = std::numeric_limits<float>::min();

//        for (const ge::Vertex& point : points)
//        {
//            min_x = std::min(min_x, point.pos.x);
//            max_x = std::max(max_x, point.pos.x);

//            min_y = std::min(min_y, point.pos.y);
//            max_y = std::max(max_y, point.pos.y);
//        }

//        const float width_in_model_space = max_x - min_x;
//        const float height_in_model_space = max_y - min_y;

//        const float width_scale = width_in_model_space / static_cast<float>(window_width);
//        const float height_scale = height_in_model_space / static_cast<float>(window_height);

//        return std::min(width_scale, height_scale);
//    }

//    namespace triangle
//    {
//        constexpr std::array points
//        {
//            ge::Vertex{{0.f, 0.5f}}
//            , ge::Vertex{{-0.5f, -0.5f}}
//            , ge::Vertex{{0.5f, -0.5f}}
//        };
//        constexpr std::array vertices
//        {
//            ge::Graph::Vertice{.index = 0, .color = ge::Color{{1.f, 0.f, 0.f}}}
//          , ge::Graph::Vertice{.index = 1, .color = ge::Color{{0.f, 1.f, 0.f}}}
//          , ge::Graph::Vertice{.index = 2, .color = ge::Color{{0.f, 0.f, 1.f}}}
//        };
//        constexpr std::array arcs
//        {
//            ge::Graph::Arc{.index_from = 0, .index_to = 1, .color = ge::Color{{1.f, 0.f, 0.f}}}
//          , ge::Graph::Arc{.index_from = 1, .index_to = 2, .color = ge::Color{{0.f, 1.f, 0.f}}}
//          , ge::Graph::Arc{.index_from = 2, .index_to = 0, .color = ge::Color{{0.f, 0.f, 1.f}}}
//        };
//    }
}

int main(/*int argc, char* argv[]*/)
{
    using namespace ge;
    using namespace ge::graph;

#ifdef GE_DEBUG_LAYERS_ENABLED
    // NOTE: add path to validation libs to LD_LIBRARY_PATH
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

//        window.start_display();

//    if (argc == 1)
//    {
//        const ge::Graph graph
//        {
//            triangle::points
//          , triangle::vertices
//          , triangle::arcs
//        };

//        render.set_object_to_draw(graph);

//        const glm::vec2 camera_pos = camera_on_center(triangle::points);
//        render.set_camera_pos(camera_pos);
//        render.set_camera_scale(1.f / 300.f);
//    }
//    else
//    {
//        std::string_view graph_file_path{argv[1]};
//        const std::vector<std::vector<ge::Vertex>> raw_points = read_points(graph_file_path);
//        auto [points, arcs] = init_arcs(raw_points);

//        const std::vector<ge::Graph::Vertice> vertices = init_vertices(points);

//        const ge::Graph graph
//        {
//            points,
//            vertices,
////            {},
//            arcs
//        };

//        render.set_object_to_draw(graph);

//        const glm::vec2 camera_pos = camera_on_center(points);
//        render.set_camera_pos(camera_pos);

//        render.set_camera_scale(scale_to_fit_all(points, width, height));
//    }

//    render.draw_frame();

        ge::RenderLoop render_loop(window, render);
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
