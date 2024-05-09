#include "ge/render/3d_image/render_3d_image.h"
#include "ge/io/read_image.h"
#include "ge/io/read_model.h"
#include "ge/common/exception.h"
#include "ge/render/camera/camera_3d_mover_fps.h"
#include "ge/window/linux/window_xcb.h"
#include "ge/render_loop/render_3d_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <iostream>
#include <fstream>
#include <span>
#include <thread>

using C = ge::World3dCoords;
using T = ge::image3d::Polygons::Triangle;
using V = ge::image3d::Polygons::TexturedVertex;

namespace square
{
    [[ maybe_unused ]] constexpr std::array vertices
    {
        V{C{{0.f, 0.f, 0.f}}, {{0.f, 1.f}}},
        V{C{{1.f, 0.f, 0.f}}, {{1.f, 1.f}}},
        V{C{{1.f, 1.f, 0.f}}, {{1.f, 0.f}}},
        V{C{{0.f, 1.f, 0.f}}, {{0.f, 0.f}}},
    };

    [[ maybe_unused ]] constexpr std::array triangles
    {
        T{{0, 1, 2}},
        T{{2, 3, 0}},
    };
}

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace ge;
    using namespace ge::image3d;

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
                LogType::Warning,
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

        Render3dImage render
        (
            ge::SurfaceParams
            {
                .surface = XcbSurface{window.get_connection(), window.get_window()}
                , .width = width
                , .height = height
                , .background_color = background_color
            },
            Msaa{.samples = MsaaSamples::x8, .enable_sample_shading = false},
            logger
        );
        Camera3dMoverFps camera_mover{render};

        window.start_display();

        std::array<Polygons, 1> models;
        std::optional<Image> texture;

        const bool draw_square = true;
        const bool draw_model = true;

        if (draw_square)
        {
            models[0] = Polygons
            {
                {square::vertices.cbegin(), square::vertices.cend()},
                {square::triangles.cbegin(), square::triangles.cend()}
            };
            texture.emplace(read_image("../res/dwarf_king.jpg"));

            World3dCoords obj_pos{{-1.f, -1.f, -1.f}};
            render.set_object_to_draw
            (
                ObjectTransform
                {
                    .pos = obj_pos,
                    .rotate = glm::vec3{0.f},
                    .scale = glm::vec3{1.f}
                },
                models,
                *texture
            );
            render.get_camera().set_pos(World3dCoords{{0.f, 0.f, 0.f}});
            camera_mover.look_at(obj_pos);
        }
        else if (draw_model)
        {
            models[0] = from_model(read_model_obj("../res/viking_room.obj"));
            texture.emplace(read_image("../res/viking_room.png"));
            World3dCoords obj_pos{{-1.f, -1.f, -1.f}};
            render.set_object_to_draw
            (
                ObjectTransform
                {
                    .pos = obj_pos,
                    .rotate = glm::vec3{-90.f, -90.f, 0.f},
                    .scale = glm::vec3{1.f}
                },
                models,
                *texture
            );

            render.get_camera().set_pos(World3dCoords{{0.f, 0.f, 0.f}});
            camera_mover.look_at(obj_pos);
        }

        assert(texture.has_value());

        render.draw_frame();

        Render3dLoop render_loop{window, render, camera_mover};
        while (not render_loop.stopped())
        {
            render_loop.handle_window_events();
            render.draw_frame();
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
