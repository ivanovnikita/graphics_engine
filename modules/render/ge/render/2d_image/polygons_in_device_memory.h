#pragma once

#include "polygons.h"
#include "ge/render/vulkan_common/device.h"
#include "ge/render/vulkan_common/buffer.h"

#include <vulkan/vulkan.hpp>

namespace ge::image2d
{
    /*
        Polygons in memory:
        [[triangle point; texture coord]:

        [[[vec2; vec2]; [vec2; vec2]; [vec2; vec2]], ...]]
    */
    struct PolygonsInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize offset;
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
