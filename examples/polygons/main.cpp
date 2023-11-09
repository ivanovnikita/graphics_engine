#include "ge/render/2d_tiles/render_2d_tiles.h"
#include "ge/common/exception.h"
#include "ge/window/linux/window_xcb.h"
#include "ge/render_loop/render_2d_loop.h"

#ifdef GE_DEBUG_LAYERS_ENABLED
#include "vk_layer_path.h"
#endif

#include <iostream>
#include <fstream>
#include <span>
#include <thread>

using C = ge::World2dCoords;
using T = ge::tiles::Polygons::Triangle;
using L = ge::tiles::Polygons::Line;
using Cl = ge::Color;

constexpr Cl WH{{1.f, 1.f, 1.f}};
constexpr Cl BL{{0.f, 0.f, 0.f}};

namespace square
{
    [[ maybe_unused ]] constexpr std::array points
    {
        C{{0.f, 0.f}},
        C{{1.f, 0.f}},
        C{{1.f, 1.f}},
        C{{0.f, 1.f}},
    };

    [[ maybe_unused ]] constexpr std::array triangles
    {
        T{{0, 1, 2}, WH},
        T{{2, 3, 0}, WH},
    };

    [[ maybe_unused ]] constexpr std::array lines
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 0}, BL},
        L{{2, 3}, BL},
        L{{3, 0}, BL},
    };
}

namespace hex
{
    const std::vector<ge::World2dCoords> points
    {
        C{{0.f, 2.f}},
        C{{1.f, 0.f}},
        C{{3.f, 0.f}},
        C{{4.f, 2.f}},
        C{{3.f, 4.f}},
        C{{1.f, 4.f}},
    };

    const std::vector<ge::tiles::Polygons::Triangle> triangles
    {
        T{{0, 1, 2}, WH},
        T{{3, 4, 5}, WH},
        T{{2, 3, 5}, WH},
        T{{5, 0, 2}, WH},
    };

    const std::vector<ge::tiles::Polygons::Line> border
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{3, 4}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},
    };

    const std::vector<ge::tiles::Polygons::Line> lines_1
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{3, 4}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},

        L{{0, 3}, BL},
        L{{1, 4}, BL},
        L{{2, 5}, BL},
    };

    const std::vector<ge::tiles::Polygons::Line> lines_2
    {
        L{{0, 1}, BL},
        L{{2, 3}, BL},
        L{{3, 4}, BL},
        L{{5, 0}, BL},

        L{{1, 4}, BL},
        L{{2, 5}, BL},
    };

    const std::vector<ge::tiles::Polygons::Line> lines_3
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{3, 4}, BL},
        L{{4, 5}, BL},

        L{{0, 3}, BL},
        L{{2, 5}, BL},
    };

    const std::vector<ge::tiles::Polygons::Line> lines_4
    {
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},

        L{{0, 3}, BL},
        L{{1, 4}, BL},
    };
}

namespace
{
    ge::tiles::Polygons move_object
    (
        const ge::tiles::Polygons& x,
        const glm::vec2& offset
    )
    {
        ge::tiles::Polygons result = x;

        for (ge::World2dCoords& vertex : result.points)
        {
            vertex.coords += offset;
        }

        return result;
    }
}

int main(int argc, char* /*argv*/[])
{
    using namespace ge;
    using namespace ge::tiles;

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

        constexpr uint16_t width = 500;
        constexpr uint16_t height = 500;
        constexpr DynamicSize size
        {
            .default_size = ge::Size{width, height}
            , .min_size = ge::Size{100, 100}
            , .max_size = std::nullopt
        };
    //    constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};
        constexpr std::array<uint8_t, 4> background_color{100, 100, 100, 1};

        auto window = WindowXCB(size, background_color, logger);

        Render2dTiles render
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
            const Polygons polygons
            {
                hex::points,
                hex::triangles,
                hex::border
            };

            std::vector<Polygons> values;
            values.emplace_back(polygons);
            values.emplace_back(move_object(polygons, {3, 2}));
            values.emplace_back(move_object(polygons, {3, -2}));
            values.emplace_back(move_object(polygons, {-3, 2}));
            values.emplace_back(move_object(polygons, {-3, -2}));
            values.emplace_back(move_object(polygons, {0, 4}));
            values.emplace_back(move_object(polygons, {0, -4}));

            const Polygons p_l_1
            {
                hex::points,
                hex::triangles,
                hex::lines_1
            };
            values.emplace_back(move_object(p_l_1, {0, 8}));
            values.emplace_back(move_object(p_l_1, {3, 6}));
            values.emplace_back(move_object(p_l_1, {-3, 6}));
            values.emplace_back(move_object(p_l_1, {0, 12}));
            values.emplace_back(move_object(p_l_1, {3, 10}));
            values.emplace_back(move_object(p_l_1, {-3, 10}));

            const Polygons p_l_2
            {
                hex::points,
                hex::triangles,
                hex::lines_2
            };
            values.emplace_back(move_object(p_l_2, {0, -8}));
            values.emplace_back(move_object(p_l_2, {3, -6}));
            values.emplace_back(move_object(p_l_2, {-3, -6}));
            values.emplace_back(move_object(p_l_2, {0, -12}));
            values.emplace_back(move_object(p_l_2, {3, -10}));
            values.emplace_back(move_object(p_l_2, {-3, -10}));

            values.emplace_back(move_object(p_l_2, {-15, 2}));
            values.emplace_back(move_object(p_l_2, {-15, -2}));
            values.emplace_back(move_object(p_l_2, {-18, 4}));
            values.emplace_back(move_object(p_l_2, {-18, 0}));
            values.emplace_back(move_object(p_l_2, {-18, -4}));
            values.emplace_back(move_object(p_l_2, {-21, 2}));
            values.emplace_back(move_object(p_l_2, {-21, -2}));
            values.emplace_back(move_object(p_l_2, {-24, 4}));
            values.emplace_back(move_object(p_l_2, {-24, 0}));
            values.emplace_back(move_object(p_l_2, {-24, -4}));
            values.emplace_back(move_object(p_l_2, {-27, 2}));
            values.emplace_back(move_object(p_l_2, {-27, -2}));
            values.emplace_back(move_object(p_l_2, {-30, 0}));

            const Polygons p_l_3
            {
                hex::points,
                hex::triangles,
                hex::lines_3
            };
            values.emplace_back(move_object(p_l_3, {6, 0}));
            values.emplace_back(move_object(p_l_3, {6, 4}));
            values.emplace_back(move_object(p_l_3, {6, -4}));
            values.emplace_back(move_object(p_l_3, {9, 2}));
            values.emplace_back(move_object(p_l_3, {9, -2}));
            values.emplace_back(move_object(p_l_3, {12, 0}));
            values.emplace_back(move_object(p_l_3, {12, 4}));
            values.emplace_back(move_object(p_l_3, {12, -4}));

            values.emplace_back(move_object(p_l_3, {-9, 6}));
            values.emplace_back(move_object(p_l_3, {-6, 8}));
            values.emplace_back(move_object(p_l_3, {-12, 8}));
            values.emplace_back(move_object(p_l_3, {-9, 10}));
            values.emplace_back(move_object(p_l_3, {-6, 12}));
            values.emplace_back(move_object(p_l_3, {-12, 12}));

            values.emplace_back(move_object(p_l_3, {-9, -6}));
            values.emplace_back(move_object(p_l_3, {-6, -8}));
            values.emplace_back(move_object(p_l_3, {-12, -8}));
            values.emplace_back(move_object(p_l_3, {-9, -10}));
            values.emplace_back(move_object(p_l_3, {-6, -12}));
            values.emplace_back(move_object(p_l_3, {-12, -12}));

            values.emplace_back(move_object(p_l_3, {-15, 6}));
            values.emplace_back(move_object(p_l_3, {-15, 10}));
            values.emplace_back(move_object(p_l_3, {-18, 8}));
            values.emplace_back(move_object(p_l_3, {-21, 6}));

            const Polygons p_l_4
            {
                hex::points,
                hex::triangles,
                hex::lines_4
            };
            values.emplace_back(move_object(p_l_4, {-6, 0}));
            values.emplace_back(move_object(p_l_4, {-6, 4}));
            values.emplace_back(move_object(p_l_4, {-6, -4}));
            values.emplace_back(move_object(p_l_4, {-9, 2}));
            values.emplace_back(move_object(p_l_4, {-9, -2}));
            values.emplace_back(move_object(p_l_4, {-12, 0}));
            values.emplace_back(move_object(p_l_4, {-12, 4}));
            values.emplace_back(move_object(p_l_4, {-12, -4}));

            values.emplace_back(move_object(p_l_4, {9, 6}));
            values.emplace_back(move_object(p_l_4, {6, 8}));
            values.emplace_back(move_object(p_l_4, {12, 8}));
            values.emplace_back(move_object(p_l_4, {9, 10}));
            values.emplace_back(move_object(p_l_4, {6, 12}));
            values.emplace_back(move_object(p_l_4, {12, 12}));

            values.emplace_back(move_object(p_l_4, {9, -6}));
            values.emplace_back(move_object(p_l_4, {6, -8}));
            values.emplace_back(move_object(p_l_4, {12, -8}));
            values.emplace_back(move_object(p_l_4, {9, -10}));
            values.emplace_back(move_object(p_l_4, {6, -12}));
            values.emplace_back(move_object(p_l_4, {12, -12}));

            values.emplace_back(move_object(p_l_4, {-15, -6}));
            values.emplace_back(move_object(p_l_4, {-15, -10}));
            values.emplace_back(move_object(p_l_4, {-18, -8}));
            values.emplace_back(move_object(p_l_4, {-21, -6}));

            render.set_object_to_draw(values);

            Camera2d camera = render.get_camera();
            camera.camera_on_center(hex::points);

            camera.set_scale(1.f / 27.f);

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
