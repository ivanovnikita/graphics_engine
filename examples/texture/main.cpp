#include "ge/render/2d_image/render_2d_image.h"
#include "ge/io/read_image.h"
#include "ge/common/exception.h"
#include "ge/window/linux/window_xcb.h"
#include "ge/render_loop/render_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <iostream>
#include <fstream>
#include <span>
#include <thread>

using C = ge::Vertex;
using T = ge::image::Polygons::Triangle;
using V = ge::image::Polygons::TexturedVertex;


namespace square
{
    [[ maybe_unused ]] constexpr std::array points
    {
        C{{-0.5f, -0.5f}},
        C{{0.5f, -0.5f}},
        C{{0.5f, 0.5f}},
        C{{-0.5f, 0.5f}},
    };

    [[ maybe_unused ]] constexpr std::array triangles
    {
        T{{V{0, {0.f, 1.f}}, V{1, {1.f, 1.f}}, V{2, {1.f, 0.f}}}},
        T{{V{2, {1.f, 0.f}}, V{3, {0.f, 0.f}}, V{0, {0.f, 1.f}}}},
    };
}

namespace
{
    [[ maybe_unused ]] float scale_to_fit_all
    (
        const std::span<const ge::Vertex>& points,
        const uint16_t window_width,
        const uint16_t window_height
    )
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

        const float width_in_model_space = max_x - min_x;
        const float height_in_model_space = max_y - min_y;

        const float width_scale = width_in_model_space / static_cast<float>(window_width);
        const float height_scale = height_in_model_space / static_cast<float>(window_height);

        return std::min(width_scale, height_scale);
    }

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

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace ge;
    using namespace ge::image;

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
            .default_size = ge::Size{width, height}
            , .min_size = ge::Size{100, 100}
            , .max_size = std::nullopt
        };
        constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};

        auto window = WindowXCB(size, background_color, logger);

        Render2dImage render
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

        const Polygons polygons
        {
            {square::points.cbegin(), square::points.cend()},
            {square::triangles.cbegin(), square::triangles.cend()}
        };

        const std::array values{polygons};

        const Image image = read_image("../res/dwarf_king.jpg");

        render.set_object_to_draw(values, image);

        const glm::vec2 camera_pos = camera_on_center(square::points);
        Camera2d camera = render.get_camera();
        camera.set_pos(camera_pos);
        camera.set_scale(scale_to_fit_all(square::points, width, height));

        render.set_camera(std::move(camera));

        render.draw_frame();

        RenderLoop render_loop(window, render);
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
