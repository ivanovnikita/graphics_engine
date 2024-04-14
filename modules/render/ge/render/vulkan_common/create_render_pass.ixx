module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.create_render_pass;

namespace ge
{
    export vk::UniqueRenderPass create_render_pass(const vk::Device&, const vk::RenderPassCreateInfo&);
}
