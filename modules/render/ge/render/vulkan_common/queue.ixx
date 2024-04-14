module;

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <optional>

#include <span>

export module vulkan_common.queue;

namespace ge
{
    export struct QueueFamilyIndices final
    {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> compute;
        std::optional<uint32_t> transfer;
    };

    export struct Queues final
    {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue compute;
        vk::Queue transfer;
    };

    export void submit
    (
        const vk::Queue&,
        std::span<const vk::SubmitInfo>,
        const vk::Fence&
    );
}
