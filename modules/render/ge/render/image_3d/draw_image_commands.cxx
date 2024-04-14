module;

#include <vulkan/vulkan.hpp>

module image_3d.draw_image_commands;

import vulkan_common.command_buffer;
import safe_cast;

namespace ge::image3d
{
    std::vector<vk::UniqueCommandBuffer> draw_image_commands
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool,
        const std::span<const vk::UniqueFramebuffer>& framebuffers,
        const vk::RenderPass& render_pass,
        const vk::Extent2D& extent,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& pipeline,
        const vk::PipelineLayout& pipeline_layout,
        const std::span<const vk::UniqueDescriptorSet> descriptor_sets,
        const PolygonsInDeviceMemory& polygons
    )
    {
        assert(framebuffers.size() == descriptor_sets.size());

        std::vector<vk::UniqueCommandBuffer> command_buffers = allocate_command_buffers
        (
            device,
            command_pool,
            vk::CommandBufferLevel::ePrimary,
            framebuffers.size()
        );

        const vk::CommandBufferBeginInfo begin_info = vk::CommandBufferBeginInfo{}
            .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

        const std::array clear_values
        {
            vk::ClearValue{}.setColor(background_color),
            vk::ClearValue{}.setDepthStencil({1.0f, 0})
        };

        vk::RenderPassBeginInfo render_pass_info = vk::RenderPassBeginInfo{}
            .setRenderPass(render_pass)
            .setRenderArea
            (
                vk::Rect2D{}
                    .setOffset(vk::Offset2D{0, 0})
                    .setExtent(extent)
            )
            .setClearValueCount(static_cast<uint32_t>(clear_values.size()))
            .setPClearValues(clear_values.data());

        const std::array<vk::DeviceSize, 1> vertex_offsets{0};
        constexpr uint32_t first_binding{0};
        constexpr uint32_t vertex_buffers_count{1};

        constexpr uint32_t instance_count{1};
        constexpr uint32_t first_vertex{0};
        constexpr uint32_t first_instance{0};
        constexpr uint32_t first_index{0};
        const vk::DeviceSize index_offset{0};

        for (size_t i = 0; i < command_buffers.size(); ++i)
        {
            const vk::CommandBuffer& command_buffer = *command_buffers[i];

            begin(command_buffer, begin_info);

            render_pass_info.setFramebuffer(*framebuffers[i]);
            command_buffer.beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);

            command_buffer.bindVertexBuffers
            (
                first_binding,
                vertex_buffers_count,
                &*polygons.vertex_buffer.buffer,
                vertex_offsets.data()
            );
            command_buffer.bindIndexBuffer
            (
                *polygons.index_buffer.buffer,
                index_offset,
                vk::IndexType::eUint32
            );

            // Each pipeline object can use up to maxBoundDescriptorSets (32)
            command_buffer.bindDescriptorSets
            (
                vk::PipelineBindPoint::eGraphics,
                pipeline_layout,
                0, // first set
                1, // descritptors count
                &*descriptor_sets[i],
                0, // dynamic offset count
                nullptr // dynamic offsets
            );

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);


            command_buffer.drawIndexed
            (
                safe_cast<uint32_t>(polygons.indices_count),
                instance_count,
                first_index,
                first_vertex,
                first_instance
            );

            command_buffer.endRenderPass();


            end(command_buffer);
        }

        return command_buffers;
    }
}
