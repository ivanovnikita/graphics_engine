#include "ge/geometry/cs_square_flat.hpp"
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
    constexpr float square_tilt = 0.f;

    [[ maybe_unused ]] const std::vector<ge::Vertex> points_flat
    {
        C{{square_width / 2.f -  square_tilt / 2.f, -square_height / 2.f}},
        C{{square_width / 2.f + square_tilt / 2.f, square_height / 2.f}},
        C{{-square_width / 2.f + square_tilt / 2.f, square_height / 2.f}},
        C{{-square_width / 2.f - square_tilt / 2.f, -square_height / 2.f}}
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

    std::string print_coords(const ge::SquareCoordAxialFlat& square, const glm::vec2& pixel)
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

    auto window = Window::create(size, background_color);

    Render render
    (
        SurfaceParams
        {
            .surface_creator = [&window] (const vk::Instance& instance)
            {
                return window->create_surface(instance);
            }
            , .width = width
            , .height = height
            , .background_color = background_color
        },
        ge::DrawMode::POLYGONS
    );

    window->start_display();

    RenderLoop render_loop(*window, render);

    const CsSquareFlat cs_square_flat
    (
        square::square_width,
        square::square_height,
        square::square_tilt
    );

    std::optional<SquareCoordAxialFlat> prev_selected_square_flat;
    const Polygons square_flat
    {
        square::points_flat,
        square::triangles,
        square::border
    };
    const Polygons selected_square_flat
    {
        square::points_flat,
        square::selected_triangles,
        square::border
    };

    std::vector<Polygons> fixed_grid_flat;

    constexpr int hex_map_radius = 11;
    for (int y = 0; y < hex_map_radius; ++y)
    {
        int y_offset = static_cast<int>(std::floor(y / 2));
        for (int x = -y_offset; x < hex_map_radius - y_offset; ++x)
        {
            const Point2dF pos_flat = cs_square_flat.to_draw_space(SquareCoordAxialFlat{x, y});
            fixed_grid_flat.emplace_back(move_object(square_flat, {pos_flat.x, pos_flat.y}));
        }
    }

    fixed_grid_flat.emplace_back(selected_square_flat);

    render.set_object_to_draw(fixed_grid_flat);

    const auto draw_selected_hex_flat =
    [
        &selected_square_flat,
        &render,
        &cs_square_flat,
        &prev_selected_square_flat,
        &fixed_grid_flat,
        &window
    ] (const MouseMoveEvent& event)
    {
        const SquareCoordAxialFlat selected_hex_pos = cs_square_flat.to_axial
        (
            Point2dF{event.pos.x, event.pos.y}
        );

        window->set_window_title(print_coords(selected_hex_pos, event.pos));

        if (selected_hex_pos == prev_selected_square_flat)
        {
            return RenderLoop::NeedRedraw::No;
        }

        prev_selected_square_flat = selected_hex_pos;

        const Point2dF pos = cs_square_flat.to_draw_space(selected_hex_pos);
        fixed_grid_flat.back() = move_object(selected_square_flat, {pos.x, pos.y});

        render.set_object_to_draw(fixed_grid_flat);

        return RenderLoop::NeedRedraw::Yes;
    };

    const glm::vec2 camera_pos = camera_on_center(square::points_flat);
    render.set_camera_pos(camera_pos);

    render.set_camera_scale(1.f / 27.f);

    render.draw_frame();

    render_loop.set_mouse_move_callback({draw_selected_hex_flat});

    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
