#pragma once

#include <vulkan/vulkan.hpp>

namespace ge
{
    vk::UniqueRenderPass create_render_pass(const vk::Device&, const vk::RenderPassCreateInfo&);
}
