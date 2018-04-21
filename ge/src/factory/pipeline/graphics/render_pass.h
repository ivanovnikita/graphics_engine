#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::pipeline::graphics::render_pass
{

    vk::UniqueRenderPass create(const vk::Device&, const vk::Format&);

}
