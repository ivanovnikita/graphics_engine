module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <span>
#include <vector>

export module vulkan_common.framebuffer;

export import vulkan_common.swapchain;
export import vulkan_common.depth_buffer;

import vulkan_common.exception;

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

module : private;

namespace ge
{
    vk::UniqueFramebuffer create_framebuffer
    (
        const vk::Device& device,
        const vk::RenderPass& render_pass,
        const vk::ImageView& surface_image,
        const vk::ImageView& depth_buffer,
        std::optional<vk::ImageView> msaa_buffer,
        const vk::Extent2D& extent
    )
    {
        std::array attachments
        {
            surface_image,
            depth_buffer,
            surface_image // в случае msaa первым идет буфер для msaa, а в конце буфер для презентации
        };

        vk::FramebufferCreateInfo framebuffer_info = vk::FramebufferCreateInfo{}
            .setRenderPass(render_pass)
            .setPAttachments(attachments.data())
            .setWidth(extent.width)
            .setHeight(extent.height)
            .setLayers(1);

        if (msaa_buffer.has_value())
        {
            attachments[0] = *msaa_buffer;
            framebuffer_info.setAttachmentCount(static_cast<uint32_t>(attachments.size()));
        }
        else
        {
            framebuffer_info.setAttachmentCount(static_cast<uint32_t>(attachments.size() - 1));
        }

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
        const SwapchainData& swapchain_data,
        const DepthBuffer& depth_buffer,
        std::optional<vk::ImageView> msaa_buffer
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
                    *depth_buffer.image_data.image_view,
                    msaa_buffer,
                    swapchain_data.extent
                )
            );
        }

        return framebuffers;
    }
}