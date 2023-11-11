#include "render_pass.h"
#include "create_render_pass.h"

namespace ge
{
    vk::UniqueRenderPass create_render_pass_default
    (
        const vk::Device& device,
        const vk::Format& present_format,
        const vk::Format& depth_format
    )
    {
        const vk::AttachmentDescription color_attachment = vk::AttachmentDescription{}
            .setFormat(present_format)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

        const vk::AttachmentReference color_attachment_ref = vk::AttachmentReference{}
            .setAttachment(0) // first attachment in array
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        const vk::AttachmentDescription depth_attachment = vk::AttachmentDescription{}
            .setFormat(depth_format)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        const vk::AttachmentReference depth_attachment_ref = vk::AttachmentReference{}
            .setAttachment(1)
            .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        // Один проход рендера может состоять из множества подпроходов (subpasses). Подпроходы — это
        // последовательные операции рендеринга, зависящие от содержимого фреймбуферов в предыдущих проходах.
        // К ним относятся, например, эффекты постобработки, применяемые друг за другом. Если объединить их в
        // один проход рендера, Vulkan сможет перегруппировать операции для лучшего сохранения пропускной
        // способности памяти и большей производительности (видимо, имеется в виду тайловый рендеринг).
        const vk::SubpassDescription subpass = vk::SubpassDescription{}
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)

            // The index of the attachment in this array is directly
            // referenced from the fragment shader with the
            // 'layout(location = 0) out vec4 outColor' directive in fragment shader.
            // (ссылается именно на порядковый номер буфера в массиве)
            // (типа если location = 1, то это отсылка ко второму буферу в ColorAttachments?)
            .setPColorAttachments(&color_attachment_ref)
            .setPDepthStencilAttachment(&depth_attachment_ref);

        const vk::SubpassDependency dependency = vk::SubpassDependency{}
            .setSrcSubpass(VK_SUBPASS_EXTERNAL)
            .setDstSubpass(0)
            .setSrcStageMask
            (
                vk::PipelineStageFlagBits::eColorAttachmentOutput |
                vk::PipelineStageFlagBits::eEarlyFragmentTests
            )
            .setSrcAccessMask(vk::AccessFlags{})
            .setDstStageMask
            (
                vk::PipelineStageFlagBits::eColorAttachmentOutput |
                vk::PipelineStageFlagBits::eEarlyFragmentTests
            )
            .setDstAccessMask
            (
//                vk::AccessFlagBits::eColorAttachmentRead |
                vk::AccessFlagBits::eColorAttachmentWrite |
                vk::AccessFlagBits::eDepthStencilAttachmentWrite
            );

        const std::array attachments
        {
            color_attachment,
            depth_attachment
        };

        const vk::RenderPassCreateInfo render_pass_info = vk::RenderPassCreateInfo{}
            .setAttachmentCount(static_cast<uint32_t>(attachments.size()))
            .setPAttachments(attachments.data())
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency);

        return create_render_pass(device, render_pass_info);
    }
}
