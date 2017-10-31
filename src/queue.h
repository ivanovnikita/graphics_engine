#pragma once

#include <vulkan/vulkan.hpp>

#include <cstdint>

namespace ge::impl
{

    struct QueueFamilyIndices
    {
        uint32_t graphics;
        uint32_t present;
        uint32_t compute;
        uint32_t transfer;
    };

    struct Queues
    {
        vk::Queue graphics;
        vk::Queue present;
        vk::Queue compute;
        vk::Queue transfer;
    };

} // namespace ge::impl
