#pragma once

#include "polygons.h"
#include "ge/render/vulkan_common/device.h"
#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

namespace ge::image
{
    /*
        Polygons in memory:
        [triangle points: [[vec2; vec2; vec2], ...]]
        [texture coords: [[vec2; vec2; vec2], ...]]
    */
    struct PolygonsInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize triangle_points_offset;
        vk::DeviceSize texture_coords_offset;
        size_t vertices_count;
    };

    PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>&
    );
}
