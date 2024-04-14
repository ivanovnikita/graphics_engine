module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.polygons_in_device_memory;

export import tiles_2d.polygons;
export import vulkan_common.device;
export import vulkan_common.buffer;

namespace ge::tiles
{
    /*
        Polygons in memory:
        [triangle points: [[vec2; vec2; vec2], ...]]
        [line points: [[vec2; vec2], ...]]
        [triangle colors: [[u8vec; u8vec; u8vec], ...]]
        [line colors: [[u8vec; u8vec], ...]]
        Цвет хранится отдельным блоком для возможности его быстрого изменения
    */
    export struct PolygonsInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize triangle_points_offset;
        vk::DeviceSize line_points_offset;
        vk::DeviceSize triangle_colors_offset;
        vk::DeviceSize line_colors_offset;
        size_t triangle_points_count;
        size_t line_points_count;
    };

    export PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>&
    );
}
