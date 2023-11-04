#include "render_pass.h"
#include "create_render_pass.h"

namespace ge
{
    vk::UniqueRenderPass create_render_pass_default
    (
        const vk::Device& device,
        const vk::Format& format
    )
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

        // Один проход рендера может состоять из множества подпроходов (subpasses). Подпроходы — это
        // последовательные операции рендеринга, зависящие от содержимого фреймбуферов в предыдущих проходах.
        // К ним относятся, например, эффекты постобработки, применяемые друг за другом. Если объединить их в
        // один проход рендера, Vulkan сможет перегруппировать операции для лучшего сохранения пропускной
        // способности памяти и большей производительности (видимо, имеется в виду тайловый рендеринг).
        const auto subpass = vk::SubpassDescription()
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)

            // The index of the attachment in this array is directly
            // referenced from the fragment shader with the
            // 'layout(location = 0) out vec4 outColor' directive in fragment shader.
            // (ссылается именно на порядковый номер буфера в массиве)
            // (типа если location = 1, то это отсылка ко второму буферу в ColorAttachments?)
            .setPColorAttachments(&color_attachment_ref);

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

        return create_render_pass(device, render_pass_info);
    }
}
