#include "framebuffer.h"

namespace ge::impl::factory::framebuffer
{

    vk::UniqueFramebuffer create
    (
        const vk::Device& logical_device
      , const vk::RenderPass& render_pass
      , const vk::ImageView& image_view
      , const vk::Extent2D& extent
    )
    {
        const auto framebuffer_info = vk::FramebufferCreateInfo()
            .setRenderPass(render_pass)
            .setAttachmentCount(1)
            .setPAttachments(&image_view)
            .setWidth(extent.width)
            .setHeight(extent.height)
            .setLayers(1);

        return logical_device.createFramebufferUnique(framebuffer_info);
    }

}
