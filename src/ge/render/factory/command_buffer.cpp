#include "ge/render/factory/command_buffer.h"

namespace ge::factory
{
    std::vector<vk::CommandBuffer> create_command_buffer
    (
        const vk::Device& logical_device
        , const vk::CommandPool& command_pool
        , const std::vector<vk::UniqueFramebuffer>& framebuffes
        , const vk::RenderPass& render_pass
        , const vk::Extent2D& extent
        , const vk::Pipeline& pipeline
    )
    {
        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setCommandPool(command_pool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(static_cast<uint32_t>(framebuffes.size()));

        const std::vector<vk::CommandBuffer> command_buffers = logical_device.allocateCommandBuffers(alloc_info);

        for (size_t i = 0; i < command_buffers.size(); ++i)
        {
            const auto begin_info = vk::CommandBufferBeginInfo{}
                .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
            command_buffers[i].begin(begin_info);

            const vk::ClearValue clear_color
            {
                vk::ClearColorValue{std::array<float,4>{0.0f, 0.0f, 0.0f, 1.0f}}
            };
            const auto render_pass_info = vk::RenderPassBeginInfo{}
                .setRenderPass(render_pass)
                .setFramebuffer(*framebuffes[i])
                .setRenderArea
                (
                    vk::Rect2D{}
                        .setOffset(vk::Offset2D{0, 0})
                        .setExtent(extent)
                )
                .setClearValueCount(1)
                .setPClearValues(&clear_color);
            command_buffers[i].beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);

            command_buffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

            constexpr uint32_t vertex_count = 3;
            constexpr uint32_t instance_count = 1;
            constexpr uint32_t first_vertex = 0;
            constexpr uint32_t first_instance = 0;
            command_buffers[i].draw
            (
                vertex_count
                , instance_count
                , first_vertex
                , first_instance
            );

            command_buffers[i].endRenderPass();

            command_buffers[i].end();
        }

        return command_buffers;
    }
}
