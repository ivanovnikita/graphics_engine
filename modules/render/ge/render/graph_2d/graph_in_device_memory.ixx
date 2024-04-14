module;

#include <vulkan/vulkan.hpp>

export module graph_2d.graph_in_device_memory;

export import graph_2d.graph_2d;
export import vulkan_common.device;
export import vulkan_common.buffer;

namespace ge::graph
{
    /*
        Graph in memory:
        [arc points: [vec2; vec2], ...]
        [vertice points: [vec2, ...]]
        [arc colors: [u8vec2; u8vec2], ...] // (duplicated for each vertice in arc)
        [vertice colors: [u8vec, ...]]
    */
    export struct GraphInDeviceMemory final
    {
        BufferData buffer;
        vk::DeviceSize arc_points_offset;
        vk::DeviceSize vertice_points_offset;
        vk::DeviceSize arc_colors_offset;
        vk::DeviceSize vertice_colors_offset;
        size_t arc_points_count;
        size_t vertice_points_count;
    };

    export GraphInDeviceMemory load_graph_to_device
    (
        const DeviceData&,
        const vk::CommandPool&,
        const vk::Fence& transfer_finished,
        const Graph&
    );

    // TODO: add func for partial update of colors
    // TODO: add func for full update of colors
}
