#include "ge/render/factory/command_buffer.h"
#include "ge/render/utils/safe_cast.hpp"

namespace ge::factory
{
    std::vector<vk::CommandBuffer> create_command_buffer
    (
        const vk::Device& logical_device
        , const vk::CommandPool& command_pool
        , const std::span<const vk::UniqueFramebuffer>& framebuffes
        , const vk::RenderPass& render_pass
        , const vk::Extent2D& extent
        , const vk::ClearColorValue& background_color
        , const vk::Pipeline& pipeline
        , const vk::PipelineLayout& pipeline_layout
        , const std::span<const vk::DescriptorSet> descriptor_sets
        , const vk::Buffer& vertices
        , const vk::Buffer& colors
        , const vk::Buffer& indices
        , const size_t indices_count
    )
    {
        assert(framebuffes.size() == descriptor_sets.size());

        const auto alloc_info = vk::CommandBufferAllocateInfo{}
            .setCommandPool(command_pool)
            .setLevel(vk::CommandBufferLevel::ePrimary)
            .setCommandBufferCount(static_cast<uint32_t>(framebuffes.size()));

        const std::vector<vk::CommandBuffer> command_buffers = logical_device.allocateCommandBuffers(alloc_info);

        const auto begin_info = vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
        const vk::ClearValue clear_color{background_color};
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

            const std::array<vk::Buffer, 2> vertex_buffers{vertices, colors};
            const std::array<vk::DeviceSize, 2> vertices_offsets{0, 0};
            constexpr uint32_t first_binding{0};
            constexpr uint32_t binding_count{vertex_buffers.size()};
            command_buffer.bindVertexBuffers
            (
                first_binding
                , binding_count
                , vertex_buffers.data() // TODO: one array with coords and colors
                , vertices_offsets.data() // TODO: different offsets for coords and colors? [all coords] [all line colors] [all point colors]
            );

            // TODO: and bind later another index buffer (by another offset in the same buffer) with indices for points
            constexpr vk::DeviceSize indices_offset{0};
            command_buffer.bindIndexBuffer
            (
                indices
                , indices_offset
                , vk::IndexType::eUint16
            );

            command_buffer.bindDescriptorSets
            (
                vk::PipelineBindPoint::eGraphics
                , pipeline_layout
                , 0 // first set
                , 1 // descritptors count
                , &descriptor_sets[i]
                , 0 // dynamic offset count
                , nullptr // dynamic offsets
            );

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);

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

            // TODO: bind another pipeline with other draw primitives

            // TODO: draw ui here

            command_buffer.endRenderPass();

            command_buffer.end();
        }

        return command_buffers;
    }
}
