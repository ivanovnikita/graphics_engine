module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.render_pass;

export import antialiasing;

import vulkan_common.create_antialiasing;
import vulkan_common.create_render_pass;
import overloaded;

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

module : private;

namespace ge
{
    vk::UniqueRenderPass create_render_pass_default
    (
        const vk::Device& device,
        const vk::Format& present_format,
        const vk::Format& depth_format,
        const Antialiasing& antialiasing
    )
    {
        vk::AttachmentDescription color_attachment = vk::AttachmentDescription{}
            .setFormat(present_format)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined);

        const vk::AttachmentReference color_attachment_ref = vk::AttachmentReference{}
            .setAttachment(0) // first attachment in array
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        vk::AttachmentDescription depth_attachment = vk::AttachmentDescription{}
            .setFormat(depth_format)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        std::optional<vk::AttachmentReference> color_attachment_resolve_ref;
        std::optional<vk::AttachmentDescription> color_attachment_resolve;
        std::visit
        (
            overloaded
            {
                [
                    &color_attachment,
                    &depth_attachment
                ] (NoAntialiasing)
                {
                    color_attachment.setSamples(vk::SampleCountFlagBits::e1);
                    color_attachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

                    depth_attachment.setSamples(vk::SampleCountFlagBits::e1);
                },
                [
                    &color_attachment,
                    &depth_attachment,
                    &color_attachment_resolve_ref,
                    &color_attachment_resolve,
                    &present_format
                ] (const Msaa& msaa)
                {
                    const vk::SampleCountFlagBits samples = sample_count(msaa);

                    color_attachment.setSamples(samples);
                    color_attachment.setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

                    depth_attachment.setSamples(samples);

                    color_attachment_resolve_ref.emplace
                    (
                        vk::AttachmentReference{}
                            .setAttachment(2)
                            .setLayout(vk::ImageLayout::eColorAttachmentOptimal)
                    );

                    color_attachment_resolve.emplace
                    (
                        vk::AttachmentDescription{}
                            .setFormat(present_format)
                            .setSamples(vk::SampleCountFlagBits::e1)
                            .setLoadOp(vk::AttachmentLoadOp::eDontCare)
                            .setStoreOp(vk::AttachmentStoreOp::eStore)
                            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                            .setInitialLayout(vk::ImageLayout::eUndefined)
                            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
                    );
                }
            }
            , antialiasing
        );

        const vk::AttachmentReference depth_attachment_ref = vk::AttachmentReference{}
            .setAttachment(1)
            .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

        // Один проход рендера может состоять из множества подпроходов (subpasses). Подпроходы — это
        // последовательные операции рендеринга, зависящие от содержимого фреймбуферов в предыдущих проходах.
        // К ним относятся, например, эффекты постобработки, применяемые друг за другом. Если объединить их в
        // один проход рендера, Vulkan сможет перегруппировать операции для лучшего сохранения пропускной
        // способности памяти и большей производительности (видимо, имеется в виду тайловый рендеринг).
        vk::SubpassDescription subpass = vk::SubpassDescription{}
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)

            // The index of the attachment in this array is directly
            // referenced from the fragment shader with the
            // 'layout(location = 0) out vec4 outColor' directive in fragment shader.
            // (ссылается именно на порядковый номер буфера в массиве)
            // (типа если location = 1, то это отсылка ко второму буферу в ColorAttachments?)
            .setPColorAttachments(&color_attachment_ref)
            .setPDepthStencilAttachment(&depth_attachment_ref);

        if (color_attachment_resolve_ref.has_value())
        {
            subpass.setPResolveAttachments(&*color_attachment_resolve_ref);
        }

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

        std::array attachments{color_attachment, depth_attachment, vk::AttachmentDescription{}};
        vk::RenderPassCreateInfo render_pass_info = vk::RenderPassCreateInfo{}
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency)
            .setPAttachments(attachments.data());

        if (color_attachment_resolve.has_value())
        {
            attachments[2] = *color_attachment_resolve;
            render_pass_info.setAttachmentCount(static_cast<uint32_t>(attachments.size()));
        }
        else
        {
            render_pass_info.setAttachmentCount(static_cast<uint32_t>(attachments.size() - 1));
        }

        return create_render_pass(device, render_pass_info);
    }
}
