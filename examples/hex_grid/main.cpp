#include "ge/geometry/cs_hex.hpp"
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

namespace hex
{
    [[ maybe_unused ]] const std::vector<ge::Vertex> points
    {
        C{{-2.f, 0.f}},
        C{{-1.f, -2.f}},
        C{{1.f, -2.f}},
        C{{2.f, 0.f}},
        C{{1.f, 2.f}},
        C{{-1.f, 2.f}},
    };

    [[ maybe_unused ]] const std::vector<ge::Polygons::Triangle> triangles
    {
        T{{0, 1, 2}, GR},
        T{{3, 4, 5}, GR},
        T{{2, 3, 5}, GR},
        T{{5, 0, 2}, GR},
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
        L{{3, 4}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},
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
        }
    );

    window->start_display();

    const CsHex cs_hex(4.f, 4.f, -1.f, 1.f);

    std::optional<HexCoordDoubledHeight> prev_selected_hex;
    const Polygons hex
    {
        hex::points,
        hex::triangles,
        hex::border
    };

    const Polygons selected_hex
    {
        hex::points,
        hex::selected_triangles,
        hex::border
    };

    constexpr int fixed_grid_side = 23;
    std::vector<Polygons> fixed_grid;
    fixed_grid.reserve(fixed_grid_side * fixed_grid_side);
    for (int x = -fixed_grid_side / 2; x < fixed_grid_side / 2; ++x)
    {
        for (int y = -fixed_grid_side / 2; y < fixed_grid_side / 2; ++y)
        {
            if (std::abs(x % 2) != std::abs(y % 2))
            {
                continue;
            }

            const Point2dF pos = cs_hex.to_draw_space(HexCoordDoubledHeight{x, y});
            fixed_grid.emplace_back(move_object(hex, {pos.x, pos.y}));
        }
    }

    fixed_grid.emplace_back(selected_hex);

    render.set_object_to_draw(fixed_grid);

    const auto draw_seleted_hex =
    [
        &selected_hex,
        &render,
        &cs_hex,
        &prev_selected_hex,
        &fixed_grid
    ] (const glm::vec2& new_pos)
    {
        const HexCoordDoubledHeight selected_hex_pos = cs_hex.to_hex_doubled_height(Point2dF{new_pos.x, new_pos.y});
        if (selected_hex_pos == prev_selected_hex)
        {
            return;
        }

        prev_selected_hex = selected_hex_pos;
//        std::cout << selected_hex_pos.x << " " << selected_hex_pos.y << std::endl;

        const Point2dF pos = cs_hex.to_draw_space(selected_hex_pos);
        fixed_grid.back() = move_object(selected_hex, {pos.x, pos.y});

        render.set_object_to_draw(fixed_grid);
    };


    const glm::vec2 camera_pos = camera_on_center(hex::points);
    render.set_camera_pos(camera_pos);

    render.set_camera_scale(1.f / 27.f);

    render.draw_frame();

    RenderLoop render_loop(*window, render);
    render_loop.set_mouse_move_callback({draw_seleted_hex});
    while (not render_loop.stopped())
    {
        render_loop.handle_window_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
