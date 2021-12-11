#include "ge/common/logger.hpp"
#include "ge/geometry/cs_square_flat.hpp"
#include "ge/geometry/cs_square_pointy.hpp"
#include "ge/render/render_impl.h"
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

using C = ge::Vertex;
using T = ge::Polygons::Triangle;
using L = ge::Polygons::Line;
using Cl = ge::Color;

[[ maybe_unused ]] constexpr Cl WH{{1.f, 1.f, 1.f}};
[[ maybe_unused ]] constexpr Cl BL{{0.f, 0.f, 0.f}};
[[ maybe_unused ]] constexpr Cl OR{{0.835f, 0.468f, 0.2f}};
[[ maybe_unused ]] constexpr Cl VI{{0.584f, 0.454f, 0.65f}};
[[ maybe_unused ]] constexpr Cl GR{{0.415f, 0.529f, 0.349f}};

namespace square
{
    constexpr float square_width = 4.f;
    constexpr float square_height = 2.f;
    constexpr float square_tilt_y = -1.f;

    [[ maybe_unused ]] const std::vector<ge::Vertex> points_flat
    {
        C{{square_width / 2.f, -square_height / 2.f + square_tilt_y / 2.f}},
        C{{square_width / 2.f, square_height / 2.f + square_tilt_y / 2.f}},
        C{{-square_width / 2.f, square_height / 2.f - square_tilt_y / 2.f}},
        C{{-square_width / 2.f, -square_height / 2.f - square_tilt_y / 2.f}}
    };

    [[ maybe_unused ]] const std::vector<ge::Polygons::Triangle> triangles
    {
        T{{0, 1, 2}, GR},
        T{{2, 3, 0}, GR},
    };

    [[ maybe_unused ]] const std::vector<ge::Polygons::Triangle> selected_triangles = []
    {
        auto result = triangles;

        for (ge::Polygons::Triangle& tr : result)
        {
            tr.color = OR;
        }

        return result;
    } ();

    [[ maybe_unused ]] const std::vector<ge::Polygons::Line> border
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{3, 0}, BL},
    };

    [[ maybe_unused ]] const std::vector<ge::Polygons::Line> selected_border = []
    {
        auto result = border;
        for (ge::Polygons::Line& line : result)
        {
            line.color = GR;
        }
        return result;
    } ();
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

    ge::Polygons move_object
    (
        const ge::Polygons& x,
        const glm::vec2& offset
    )
    {
        ge::Polygons result = x;

        for (auto& vertex : result.points)
        {
            vertex.pos += offset;
        }

        return result;
    }

    std::string print_coords(const ge::SquareCoordAxialPointy& square, const glm::vec2& pixel)
    {
        using namespace ge;

        std::stringstream in;
        in << "square: " << square
            << " | pixel: " << pixel;

        return in.str();
    }

    enum class HexCsType
    {
        Flat,
        Pointy
    };
}

int main(int /*argc*/, char* /*argv*/[])
{
    using namespace ge;

#ifdef GE_DEBUG_LAYERS_ENABLED
    constexpr int override = 1;
    setenv("VK_LAYER_PATH", std::string{ge::VK_LAYER_PATH}.c_str(), override);
#endif

    const Logger logger
    {
        Flags<LogType>
        {
            LogType::Error,
            LogType::ErrorDetails,
            LogType::SystemInfo
        }
    };

    constexpr uint16_t width = 500;
    constexpr uint16_t height = 500;
    constexpr DynamicSize size
    {
        .default_size = Size{width, height}
        , .min_size = Size{100, 100}
        , .max_size = std::nullopt
    };
//    constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};
    constexpr std::array<uint8_t, 4> background_color{100, 100, 100, 1};
    auto window = WindowXCB(size, background_color, logger);

    RenderImpl render
    (
        SurfaceParams
        {
            .surface = XcbSurface{window.get_connection(), window.get_window()}
            , .width = width
            , .height = height
            , .background_color = background_color
        },
        ge::DrawMode::POLYGONS,
        logger
    );

    window.start_display();

    RenderLoop render_loop(window, render);

    const CsSquarePointy cs_square_pointy
    (
        square::square_width,
        square::square_height,
        square::square_tilt_y
    );

    std::optional<SquareCoordAxialPointy> prev_selected_square_pointy;
    const Polygons square_pointy
    {
        square::points_flat,
        square::triangles,
        square::border
    };
    const Polygons selected_square_pointy
    {
        square::points_flat,
        square::selected_triangles,
        square::border
    };

    std::vector<Polygons> fixed_grid_pointy;

    constexpr int hex_map_radius = 11;
    for (int y = 0; y < hex_map_radius; ++y)
    {
        int y_offset = static_cast<int>(std::floor(y / 2));
        for (int x = -y_offset; x < hex_map_radius - y_offset; ++x)
        {
            const Point2dF pos_flat = cs_square_pointy.to_draw_space(SquareCoordAxialPointy{x, y});
            fixed_grid_pointy.emplace_back(move_object(square_pointy, {pos_flat.x, pos_flat.y}));
        }
    }

    fixed_grid_pointy.emplace_back(selected_square_pointy);

    render.set_object_to_draw(fixed_grid_pointy);

    const auto draw_selected_hex_pointy =
    [
        &selected_square_pointy,
        &render,
        &cs_square_pointy,
        &prev_selected_square_pointy,
        &fixed_grid_pointy,
        &window
    ] (const MouseMoveEvent& event)
    {
        const SquareCoordAxialPointy selected_hex_pos = cs_square_pointy.to_axial
        (
            Point2dF{event.pos.x, event.pos.y}
        );

        window.set_window_title(print_coords(selected_hex_pos, event.pos));

        if (selected_hex_pos == prev_selected_square_pointy)
        {
            return RenderLoop::NeedRedraw::No;
        }

        prev_selected_square_pointy = selected_hex_pos;

        const Point2dF pos = cs_square_pointy.to_draw_space(selected_hex_pos);
        fixed_grid_pointy.back() = move_object(selected_square_pointy, {pos.x, pos.y});

        render.set_object_to_draw(fixed_grid_pointy);

        return RenderLoop::NeedRedraw::Yes;
    };

    const glm::vec2 camera_pos = camera_on_center(square::points_flat);

    Camera2d camera = render.get_camera();
    camera.set_pos(camera_pos);
    camera.set_scale(1.f / 27.f);
    render.set_camera(std::move(camera));

    render.draw_frame();

    render_loop.set_mouse_move_callback({draw_selected_hex_pointy});

    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
