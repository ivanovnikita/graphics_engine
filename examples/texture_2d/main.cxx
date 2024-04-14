#include <iostream>
#include <fstream>
#include <span>
#include <thread>

import image_2d.render_image_2d;
import io.read_image;
import exception;
import window_xcb;
import render_2d_loop;

#ifdef GE_DEBUG_LAYERS_ENABLED
import vk_layer_path;
#endif


using C = ge::World2dCoords;
using T = ge::image2d::Polygons::Triangle;
using V = ge::image2d::Polygons::TexturedVertex;


namespace square
{
    constexpr std::array points
    {
        C{{-0.5f, -0.5f}},
        C{{0.5f, -0.5f}},
        C{{0.5f, 0.5f}},
        C{{-0.5f, 0.5f}},
    };

    constexpr std::array vertices
    {
        V{points[0], {{0.f, 1.f}}},
        V{points[1], {{1.f, 1.f}}},
        V{points[2], {{1.f, 0.f}}},
        V{points[3], {{0.f, 0.f}}},
    };

    constexpr std::array triangles
    {
        T{{0, 1, 2}},
        T{{2, 3, 0}},
    };
}

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace ge;
    using namespace ge::image2d;

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

        RenderImage2d render
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
            {square::vertices.cbegin(), square::vertices.cend()},
            {square::triangles.cbegin(), square::triangles.cend()}
        };

        const std::array values{polygons};

        const Image image = read_image("../res/dwarf_king.jpg");

        render.set_object_to_draw(values, image);

        Camera2d camera = render.get_camera();
        camera.camera_on_center(square::points);
        camera.scale_to_fit_all(square::points);

        render.set_camera(std::move(camera));

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
