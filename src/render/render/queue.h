#pragma once

#include <vulkan/vulkan.hpp>

#include <cstdint>
#include <optional>

namespace ge
{

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphics;
        std::optional<uint32_t> present;
        std::optional<uint32_t> compute;
        std::optional<uint32_t> transfer;
    };

    struct Queues
    {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue compute;
        vk::Queue transfer;
    };

}
