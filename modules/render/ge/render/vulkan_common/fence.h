#pragma once

#include <vulkan/vulkan.hpp>

#include <chrono>
#include <span>

namespace ge
{
    vk::UniqueFence create_fence(const vk::Device&);
    void reset_fences(const vk::Device&, std::span<const vk::Fence>);

    // TODO: return enum Success/Timeout
    void wait_for_fences
    (
        const vk::Device&,
        std::span<const vk::Fence>,
        bool wait_all,
        std::chrono::nanoseconds timeout
    );
}
