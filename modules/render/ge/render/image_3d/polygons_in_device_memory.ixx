module;

#include <vulkan/vulkan.hpp>

export module image_3d.polygons_in_device_memory;

export import image_3d.polygons;
export import vulkan_common.device;
export import vulkan_common.buffer;

namespace ge::image3d
{
    /*
        Polygons in memory:
        [[triangle point; texture coord]:

        [[[vec3; vec2; [vec3; vec2]; [vec3; vec2]], ...]]
    */
    export struct PolygonsInDeviceMemory final
    {
        BufferData vertex_buffer;
        BufferData index_buffer;
        size_t vertices_count;
        size_t indices_count;
    };

    export PolygonsInDeviceMemory load_polygons_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const std::span<const Polygons>&
    );
}
