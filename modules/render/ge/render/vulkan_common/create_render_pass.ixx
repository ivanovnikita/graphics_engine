module;

#include "vulkan_fwds.h"

export module vulkan_common.create_render_pass;

namespace ge
{
    export vk::UniqueRenderPass create_render_pass(const vk::Device&, const vk::RenderPassCreateInfo&);
}
