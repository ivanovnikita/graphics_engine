module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.render_pass;

export import antialiasing;

namespace ge
{
    export vk::UniqueRenderPass create_render_pass_default
    (
        const vk::Device&,
        const vk::Format& present_format,
        const vk::Format& depth_format,
        const Antialiasing&
    );
}
