#include "ge/render/factory/command_buffer.h"
#include "ge/render/utils/safe_cast.hpp"

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
        , const vk::Buffer& vertices
        , const vk::Buffer& indices
        , const size_t indices_count
    )
    {
        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setCommandPool(command_pool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(static_cast<uint32_t>(framebuffes.size()));

        const std::vector<vk::CommandBuffer> command_buffers = logical_device.allocateCommandBuffers(alloc_info);

        const auto begin_info = vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
        const vk::ClearValue clear_color
        {
            vk::ClearColorValue{std::array<float,4>{0.0f, 0.0f, 0.0f, 1.0f}}
        };
        auto render_pass_info = vk::RenderPassBeginInfo{}
            .setRenderPass(render_pass)
            .setRenderArea
            (
                vk::Rect2D{}
                    .setOffset(vk::Offset2D{0, 0})
                    .setExtent(extent)
            )
            .setClearValueCount(1)
            .setPClearValues(&clear_color);

        for (size_t i = 0; i < command_buffers.size(); ++i)
        {
            const vk::CommandBuffer& command_buffer = command_buffers[i];

            command_buffer.begin(begin_info);

            render_pass_info.setFramebuffer(*framebuffes[i]);
            command_buffer.beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

            constexpr vk::DeviceSize vertices_offsets[]{0};
            constexpr uint32_t first_binding{0};
            constexpr uint32_t binding_count{1};
            command_buffer.bindVertexBuffers
            (
                first_binding
                , binding_count
                , &vertices
                , vertices_offsets
            );

            constexpr vk::DeviceSize indices_offset{0};
            command_buffer.bindIndexBuffer
            (
                indices
                , indices_offset
                , vk::IndexType::eUint16
            );

            constexpr uint32_t instance_count{1};
            constexpr uint32_t first_index{0};
            constexpr uint32_t vertex_offset{0};
            constexpr uint32_t first_instance{0};
            command_buffer.drawIndexed
            (
                safe_cast<uint32_t>(indices_count)
                , instance_count
                , first_index
                , vertex_offset
                , first_instance
            );

            command_buffer.endRenderPass();

            command_buffer.end();
        }

        return command_buffers;
    }
}
