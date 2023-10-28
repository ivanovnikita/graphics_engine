#include "ge/render/2d_tiles/render_2d_tiles.h"
#include "ge/common/exception.h"
#include "ge/geometry/cs_hex_flat.hpp"
#include "ge/geometry/cs_hex_pointy.hpp"
#include "ge/geometry/convert_between_flat_and_pointy.h"
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
using T = ge::tiles::Polygons::Triangle;
using L = ge::tiles::Polygons::Line;
using Cl = ge::Color;

[[ maybe_unused ]] constexpr Cl WH{{1.f, 1.f, 1.f}};
[[ maybe_unused ]] constexpr Cl BL{{0.f, 0.f, 0.f}};
[[ maybe_unused ]] constexpr Cl OR{{0.835f, 0.468f, 0.2f}};
[[ maybe_unused ]] constexpr Cl VI{{0.584f, 0.454f, 0.65f}};
[[ maybe_unused ]] constexpr Cl GR{{0.415f, 0.529f, 0.349f}};

namespace hex
{
    // https://www.redblobgames.com/grids/hexagons/#basics
    constexpr float hex_size_part = 1;

    [[ maybe_unused ]] const std::vector<ge::Vertex> points_flat
    {
        C{{-hex_size_part, 0.f}},
        C{{-hex_size_part / 2.f, std::sqrt(3.f) * (-hex_size_part / 2.f)}},
        C{{hex_size_part / 2.f, std::sqrt(3.f) * (-hex_size_part / 2.f)}},
        C{{hex_size_part, 0.f}},
        C{{hex_size_part / 2.f, std::sqrt(3.f) * (hex_size_part / 2.f)}},
        C{{-hex_size_part / 2.f, std::sqrt(3.f) * (hex_size_part / 2.f)}},
    };

    [[ maybe_unused ]] const std::vector<ge::Vertex> points_pointy
    {
        C{{0.f, -hex_size_part}},
        C{{std::sqrt(3.f) * (hex_size_part / 2.f), -hex_size_part / 2.f}},
        C{{std::sqrt(3.f) * (hex_size_part / 2.f), hex_size_part / 2.f}},
        C{{0.f, hex_size_part}},
        C{{std::sqrt(3.f) * (-hex_size_part / 2.f), hex_size_part / 2.f}},
        C{{std::sqrt(3.f) * (-hex_size_part / 2.f), -hex_size_part / 2.f}},
    };

    [[ maybe_unused ]] const std::vector<ge::tiles::Polygons::Triangle> triangles
    {
        T{{0, 1, 2}, GR},
        T{{3, 4, 5}, GR},
        T{{2, 3, 5}, GR},
        T{{5, 0, 2}, GR},
    };

    [[ maybe_unused ]] const std::vector<ge::tiles::Polygons::Triangle> selected_triangles = []
    {
        auto result = triangles;

        for (ge::tiles::Polygons::Triangle& tr : result)
        {
            tr.color = OR;
        }

        return result;
    } ();

    [[ maybe_unused ]] const std::vector<ge::tiles::Polygons::Line> border
    {
        L{{0, 1}, BL},
        L{{1, 2}, BL},
        L{{2, 3}, BL},
        L{{3, 4}, BL},
        L{{4, 5}, BL},
        L{{5, 0}, BL},
    };

    [[ maybe_unused ]] const std::vector<ge::tiles::Polygons::Line> selected_border = []
    {
        auto result = border;
        for (ge::tiles::Polygons::Line& line : result)
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

    ge::tiles::Polygons move_object
    (
        const ge::tiles::Polygons& x,
        const glm::vec2& offset
    )
    {
        ge::tiles::Polygons result = x;

        for (auto& vertex : result.points)
        {
            vertex.pos += offset;
        }

        return result;
    }

    std::string print_coords(const ge::HexCoordDoubledHeight& doubled, const glm::vec2& pixel)
    {
        using namespace ge;

        const HexCoordOffsetFlat offset = to_hex_offset_flat(doubled);
        const HexCoordAxialFlat axial = to_hex_axial_flat(doubled);

        std::stringstream in;
        in << "doubled: " << doubled
            << " | offset: " << offset
            << " | axial: " << axial
            << " | pixel: " << pixel;

        return in.str();
    }

    // TODO: rewrite copy-pasted code
    std::string print_coords(const ge::HexCoordDoubledWidth& doubled, const glm::vec2& pixel)
    {
        using namespace ge;

        const HexCoordOffsetPointy offset = to_hex_offset_pointy(doubled);
        const HexCoordAxialPointy axial = to_hex_axial_pointy(doubled);

        std::stringstream in;
        in << "doubled: " << doubled
            << " | offset: " << offset
            << " | axial: " << axial
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
            .default_size = Size{width, height}
            , .min_size = Size{100, 100}
            , .max_size = std::nullopt
        };
    //    constexpr std::array<uint8_t, 4> background_color{38, 38, 38, 1};
        constexpr std::array<uint8_t, 4> background_color{100, 100, 100, 1};
        auto window = WindowXCB(size, background_color, logger);

        Render2dTiles render
        (
            SurfaceParams
            {
                .surface = XcbSurface{window.get_connection(), window.get_window()}
                , .width = width
                , .height = height
                , .background_color = background_color
            },
            logger
        );

        window.start_display();

        RenderLoop render_loop(window, render);

        const CsHexFlat cs_hex_flat
        (
            2.f * hex::hex_size_part,
            static_cast<float>(std::sqrt(3) * hex::hex_size_part),
            hex::points_flat[1].pos.x,
            hex::points_flat[2].pos.x
        );
        const CsHexPointy cs_hex_pointy
        (
            static_cast<float>(std::sqrt(3) * hex::hex_size_part),
            2.f * hex::hex_size_part,
            hex::points_pointy[1].pos.y,
            hex::points_pointy[2].pos.y
        );

        std::optional<HexCoordDoubledHeight> prev_selected_hex_flat;
        const Polygons hex_flat
        {
            hex::points_flat,
            hex::triangles,
            hex::border
        };
        const Polygons selected_hex_flat
        {
            hex::points_flat,
            hex::selected_triangles,
            hex::border
        };

        std::optional<HexCoordDoubledWidth> prev_selected_hex_pointy;
        const Polygons hex_pointy
        {
            hex::points_pointy,
            hex::triangles,
            hex::border
        };
        const Polygons selected_hex_pointy
        {
            hex::points_pointy,
            hex::selected_triangles,
            hex::border
        };

        std::vector<Polygons> fixed_grid_flat;
        std::vector<Polygons> fixed_grid_pointy;

        constexpr int hex_map_radius = 11;
    //    for (int x = -hex_map_radius; x <= hex_map_radius; ++x)
    //    {
    //        const int y1 = std::max(-hex_map_radius, -x - hex_map_radius);
    //        const int y2 = std::min(hex_map_radius, -x + hex_map_radius);
    //        for (int y = y1; y <= y2; ++y)
    //        {
    //            const Point2dF pos_flat = cs_hex_flat.to_draw_space(to_hex_doubled_height(HexCoordAxialFlat{x, y}));
    //            fixed_grid_flat.emplace_back(move_object(hex_flat, {pos_flat.x, pos_flat.y}));

    //            // TODO: rewrite copy-pasted code
    //            const Point2dF pos_pointy = cs_hex_pointy.to_draw_space(to_hex_doubled_width(HexCoordAxialPointy{x, y}));
    //            fixed_grid_pointy.emplace_back(move_object(hex_pointy, {pos_pointy.x, pos_pointy.y}));
    //        }
    //    }

        for (int y = 0; y < hex_map_radius; ++y)
        {
            int y_offset = static_cast<int>(std::floor(y / 2));
            for (int x = -y_offset; x < hex_map_radius - y_offset; ++x)
            {
                const Point2dF pos_flat = cs_hex_flat.to_draw_space(to_hex_doubled_height(HexCoordAxialFlat{x, y}));
                fixed_grid_flat.emplace_back(move_object(hex_flat, {pos_flat.x, pos_flat.y}));

                // TODO: rewrite copy-pasted code
                const Point2dF pos_pointy = cs_hex_pointy.to_draw_space(to_hex_doubled_width(HexCoordAxialPointy{x, y}));
                fixed_grid_pointy.emplace_back(move_object(hex_pointy, {pos_pointy.x, pos_pointy.y}));
            }
        }

        fixed_grid_flat.emplace_back(selected_hex_flat);
        fixed_grid_pointy.emplace_back(selected_hex_pointy);

        render.set_object_to_draw(fixed_grid_flat);
        HexCsType current_cs = HexCsType::Flat;

        const auto draw_selected_hex_flat =
        [
            &selected_hex_flat,
            &render,
            &cs_hex_flat,
            &prev_selected_hex_flat,
            &fixed_grid_flat,
            &window
        ] (const MouseMoveEvent& event)
        {
            const HexCoordDoubledHeight selected_hex_pos = cs_hex_flat.to_hex_doubled_height
            (
                Point2dF{event.pos.x, event.pos.y}
            );

            window.set_window_title(print_coords(selected_hex_pos, event.pos));

            if (selected_hex_pos == prev_selected_hex_flat)
            {
                return RenderLoop::NeedRedraw::No;
            }

            prev_selected_hex_flat = selected_hex_pos;

            const Point2dF pos = cs_hex_flat.to_draw_space(selected_hex_pos);
            fixed_grid_flat.back() = move_object(selected_hex_flat, {pos.x, pos.y});

            render.set_object_to_draw(fixed_grid_flat);

            return RenderLoop::NeedRedraw::Yes;
        };

        // TODO: rewrite copy-pasted code
        const auto draw_selected_hex_pointy =
        [
            &selected_hex_pointy,
            &render,
            &cs_hex_pointy,
            &prev_selected_hex_pointy,
            &fixed_grid_pointy,
            &window
        ] (const MouseMoveEvent& event)
        {
            const HexCoordDoubledWidth selected_hex_pos = cs_hex_pointy.to_hex_doubled_width
            (
                Point2dF{event.pos.x, event.pos.y}
            );

            window.set_window_title(print_coords(selected_hex_pos, event.pos));

            if (selected_hex_pos == prev_selected_hex_pointy)
            {
                return RenderLoop::NeedRedraw::No;
            }

            prev_selected_hex_pointy = selected_hex_pos;

            const Point2dF pos = cs_hex_pointy.to_draw_space(selected_hex_pos);
            fixed_grid_pointy.back() = move_object(selected_hex_pointy, {pos.x, pos.y});

            render.set_object_to_draw(fixed_grid_pointy);

            return RenderLoop::NeedRedraw::Yes;
        };

        std::optional<HexCoordDoubledHeight> pressed_hex_flat;
        std::optional<HexCoordDoubledWidth> pressed_hex_pointy;
        const auto switch_cs_start =
        [
            &current_cs,
            &cs_hex_flat,
            &cs_hex_pointy,
            &pressed_hex_flat,
            &pressed_hex_pointy
        ] (const MouseButtonPress& e)
        {
            if (e.button != MouseButton::RIGHT)
            {
                return RenderLoop::NeedRedraw::No;
            }

            switch (current_cs)
            {
            case HexCsType::Flat:
            {
                pressed_hex_flat = cs_hex_flat.to_hex_doubled_height(Point2dF{e.pos.x, e.pos.y});
                break;
            }
            case HexCsType::Pointy:
            {
                pressed_hex_pointy = cs_hex_pointy.to_hex_doubled_width(Point2dF{e.pos.x, e.pos.y});
                break;
            }
            }

            return RenderLoop::NeedRedraw::No;
        };

        const auto switch_cs_stop =
        [
            &render,
            &render_loop,
            &current_cs,
            &cs_hex_flat,
            &cs_hex_pointy,
            &fixed_grid_flat,
            &fixed_grid_pointy,
            &pressed_hex_flat,
            &pressed_hex_pointy,
            &prev_selected_hex_flat,
            &prev_selected_hex_pointy,
            &draw_selected_hex_flat,
            &draw_selected_hex_pointy,
            &selected_hex_flat,
            &selected_hex_pointy
        ] (const MouseButtonRelease& e)
        {
            if (e.button != MouseButton::RIGHT)
            {
                return RenderLoop::NeedRedraw::No;
            }

            RenderLoop::NeedRedraw redraw = RenderLoop::NeedRedraw::No;

            switch (current_cs)
            {
            case HexCsType::Flat:
            {
                HexCoordDoubledHeight released_hex = cs_hex_flat.to_hex_doubled_height(Point2dF{e.pos.x, e.pos.y});
                if (pressed_hex_flat == released_hex)
                {
                    if (prev_selected_hex_flat.has_value())
                    {
                        prev_selected_hex_pointy = to_hex_doubled_width
                        (
                            to_pointy_by_cw_rotation(to_hex_axial_flat(*prev_selected_hex_flat))
                        );
                        const Point2dF pos = cs_hex_pointy.to_draw_space(*prev_selected_hex_pointy);
                        fixed_grid_pointy.back() = move_object(selected_hex_pointy, {pos.x, pos.y});
                    }

                    render.set_object_to_draw(fixed_grid_pointy);
                    render_loop.set_mouse_move_callback({draw_selected_hex_pointy});
                    current_cs = HexCsType::Pointy;
                    redraw = RenderLoop::NeedRedraw::Yes;
                }
                pressed_hex_flat.reset();
                break;
            }
            case HexCsType::Pointy:
            {
                HexCoordDoubledWidth released_hex = cs_hex_pointy.to_hex_doubled_width(Point2dF{e.pos.x, e.pos.y});
                if (pressed_hex_pointy == released_hex)
                {
                    if (prev_selected_hex_pointy.has_value())
                    {
                        prev_selected_hex_flat = to_hex_doubled_height
                        (
                            to_flat_by_ccw_rotation(to_hex_axial_pointy(*prev_selected_hex_pointy))
                        );
                        const Point2dF pos = cs_hex_flat.to_draw_space(*prev_selected_hex_flat);
                        fixed_grid_flat.back() = move_object(selected_hex_flat, {pos.x, pos.y});
                    }

                    render.set_object_to_draw(fixed_grid_flat);
                    render_loop.set_mouse_move_callback({draw_selected_hex_flat});
                    current_cs = HexCsType::Flat;
                    redraw = RenderLoop::NeedRedraw::Yes;
                }
                pressed_hex_pointy.reset();
                break;
            }
            }

            return redraw;
        };

        const glm::vec2 camera_pos = camera_on_center(hex::points_flat);

        Camera2d camera = render.get_camera();
        camera.set_pos(camera_pos);
        camera.set_scale(1.f / 27.f);
        render.set_camera(std::move(camera));

        render.draw_frame();

        render_loop.set_mouse_move_callback({draw_selected_hex_flat});
        render_loop.set_mouse_press_callback({switch_cs_start});
        render_loop.set_mouse_release_callback({switch_cs_stop});

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
