module;

#include "vulkan_fwds.h"

#include <vector>

export module vulkan_common.command_buffer;

export import vulkan_common.device;

namespace ge
{
    export vk::UniqueCommandBuffer allocate_command_buffer
    (
        const vk::Device&,
        const vk::CommandPool&,
        vk::CommandBufferLevel
    );

    export std::vector<vk::UniqueCommandBuffer> allocate_command_buffers
    (
        const vk::Device&,
        const vk::CommandPool&,
        vk::CommandBufferLevel,
        size_t count
    );

    export void begin(const vk::CommandBuffer&, const vk::CommandBufferBeginInfo&);
    export void end(const vk::CommandBuffer&);

    export vk::UniqueCommandBuffer create_one_time_commands
    (
        const vk::Device&,
        const vk::CommandPool&
    );

    export void submit_one_time_commands
    (
        vk::UniqueCommandBuffer,
        const DeviceData&,
        const vk::Fence&
    );
}
