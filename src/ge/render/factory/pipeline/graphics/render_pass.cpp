#include "ge/render/factory/pipeline/graphics/render_pass.h"

namespace ge::factory
{
    vk::UniqueRenderPass create_render_pass(const vk::Device& logical_device, const vk::Format& format)
    {
        const auto color_attachment = vk::AttachmentDescription()
            .setFormat(format)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

        const auto color_attachment_ref = vk::AttachmentReference()
            .setAttachment(0) // first attachment in array
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        const auto subpass = vk::SubpassDescription()
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)
            .setPColorAttachments(&color_attachment_ref); // The index of the attachment in this array is directly
                                                          // referenced from the fragment shader with the
                                                          // layout(location = 0) out vec4 outColor directive!

        const auto dependency = vk::SubpassDependency{}
            .setSrcSubpass(VK_SUBPASS_EXTERNAL)
            .setDstSubpass(0)
            .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            .setSrcAccessMask(vk::AccessFlags{})
            .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
            .setDstAccessMask
            (
                vk::AccessFlagBits::eColorAttachmentRead
                | vk::AccessFlagBits::eColorAttachmentWrite
            );

        const auto render_pass_info = vk::RenderPassCreateInfo()
            .setAttachmentCount(1)
            .setPAttachments(&color_attachment)
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency);

        return logical_device.createRenderPassUnique(render_pass_info);
    }
}
