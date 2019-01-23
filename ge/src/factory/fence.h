#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::fence
{
    vk::UniqueFence create
    (
        const vk::Device&
    );
}
