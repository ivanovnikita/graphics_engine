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

using C = ge::Vertex;
using T = ge::Polygons::Triangle;
using L = ge::Polygons::Line;
using Cl = ge::Color;

constexpr Cl WH{{1.f, 1.f, 1.f}};
constexpr Cl BL{{0.f, 0.f, 0.f}};

namespace hex
{
    const std::vector<ge::Vertex> points
    {
        C{{-2.f, 0.f}},
        C{{-1.f, -2.f}},
        C{{1.f, -2.f}},
        C{{2.f, 0.f}},
        C{{1.f, 2.f}},
        C{{-1.f, 2.f}},
    };

    const std::vector<ge::Polygons::Triangle> triangles
    {
        T{{0, 1, 2}, WH},
        T{{3, 4, 5}, WH},
        T{{2, 3, 5}, WH},
        T{{5, 0, 2}, WH},
    };

    const std::vector<ge::Polygons::Line> border
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{3, 4}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},
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

    std::optional<CoordHex> prev_selected_hex;
    const Polygons polygons
    {
        hex::points,
        hex::triangles,
        hex::border
    };

    render.set_object_to_draw({&polygons, 1});

    const CsHex cs_hex(4.f, 4.f, -1.f, 1.f);
    const auto draw_seleted_hex =
    [
        &polygons,
        &render,
        &cs_hex,
        &prev_selected_hex
    ] (const glm::vec2& new_pos)
    {
        const CoordHex selected_hex = cs_hex.convert(Point2dF{new_pos.x, new_pos.y});
        if (selected_hex == prev_selected_hex)
        {
            return;
        }

        prev_selected_hex = selected_hex;

        const Point2dF pos = cs_hex.convert(selected_hex);
        const Polygons moved_polygons = move_object(polygons, {pos.x, pos.y});

        render.set_object_to_draw({&moved_polygons, 1});
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
