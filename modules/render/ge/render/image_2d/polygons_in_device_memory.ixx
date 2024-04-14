module;

#include <vulkan/vulkan.hpp>

export module image_2d.polygons_in_device_memory;

export import image_2d.polygons;
export import vulkan_common.device;
export import vulkan_common.buffer;

namespace ge::image2d
{
    /*
        Polygons in memory:
        [[triangle point; texture coord]:

        [[[vec2; vec2]; [vec2; vec2]; [vec2; vec2]], ...]]
    */
    export struct PolygonsInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize offset;
        size_t vertices_count;
    };

    export PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>&
    );
}
