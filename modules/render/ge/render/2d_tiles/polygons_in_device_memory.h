#pragma once

#include "polygons.h"
#include "ge/render/device.h"
#include "ge/render/buffer.h"

#include <vulkan/vulkan.hpp>

namespace ge::tiles
{
    /*
        Polygons in memory:
        [triangle points: [[vec2; vec2; vec2], ...]]
        [line points: [[vec2; vec2], ...]]
        [triangle colors: [[u8vec; u8vec; u8vec], ...]]
        [line colors: [[u8vec; u8vec], ...]]
    */
    struct PolygonsInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize triangle_points_offset;
        vk::DeviceSize line_points_offset;
        vk::DeviceSize triangle_colors_offset;
        vk::DeviceSize line_colors_offset;
        size_t triangle_points_count;
        size_t line_points_count;
    };

    PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>&
    );
}
