#include "framebuffer.h"
#include "exception.h"

namespace ge
{
    vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device& device,
        const vk::RenderPass& render_pass,
        const vk::ImageView& image_view,
        const vk::Extent2D& extent
    )
    {
        const auto framebuffer_info = vk::FramebufferCreateInfo()
            .setRenderPass(render_pass)
            .setAttachmentCount(1)
            .setPAttachments(&image_view)
            .setWidth(extent.width)
            .setHeight(extent.height)
            .setLayers(1);

        vk::Framebuffer framebuffer;
        const vk::Result result = device.createFramebuffer
        (
            &framebuffer_info,
            nullptr,
            &framebuffer
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Framebuffer creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Framebuffer creation failed");
        }
        }

        return vk::UniqueFramebuffer
        {
            std::move(framebuffer),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }

    std::vector<vk::UniqueFramebuffer> create_framebuffers
    (
        const vk::Device& device,
        const vk::RenderPass& render_pass,
        const SwapchainData& swapchain_data
    )
    {
        std::vector<vk::UniqueFramebuffer> framebuffers;
        try
        {
            framebuffers.reserve(swapchain_data.image_views.size());
        }
        catch (const std::bad_alloc&)
        {
            GE_THROW_EXPECTED_ERROR("Allocation for framebuffers failed");
        }

        for (const vk::UniqueImageView& image_view : swapchain_data.image_views)
        {
            framebuffers.emplace_back
            (
                create_framebuffer
                (
                    device,
                    render_pass,
                    *image_view,
                    swapchain_data.extent
                )
            );
        }

        return framebuffers;
    }
}
