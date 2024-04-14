module;

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

export module vulkan_common.framebuffer;

export import vulkan_common.swapchain;
export import vulkan_common.depth_buffer;

namespace ge
{
    export vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device&,
        const vk::RenderPass&,
        const vk::ImageView& surface_image,
        const vk::ImageView& depth_buffer,
        std::optional<vk::ImageView> msaa_buffer,
        const vk::Extent2D&
    );

    export std::vector<vk::UniqueFramebuffer> create_framebuffers
    (
        const vk::Device&,
        const vk::RenderPass&,
        const SwapchainData&,
        const DepthBuffer&,
        std::optional<vk::ImageView> msaa_buffer
    );
}
