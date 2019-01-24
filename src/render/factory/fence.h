#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::factory
{
    vk::UniqueFence create_fence
    (
        const vk::Device&
    );
}
