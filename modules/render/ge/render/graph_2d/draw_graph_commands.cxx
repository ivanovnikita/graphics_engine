module;

#include <vulkan/vulkan.hpp>

module graph_2d.draw_graph_commands;

import vulkan_common.command_buffer;
import safe_cast;

namespace ge::graph
{
    std::vector<vk::UniqueCommandBuffer> draw_graph_commands
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool,
        const std::span<const vk::UniqueFramebuffer>& framebuffers,
        const vk::RenderPass& render_pass,
        const vk::Extent2D& extent,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& arcs_pipeline,
        const vk::Pipeline& vertices_pipeline,
        const vk::PipelineLayout& pipeline_layout,
        const std::span<const vk::UniqueDescriptorSet> descriptor_sets,
        const GraphInDeviceMemory& graph
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

        const std::array graph_buffers{*graph.buffer.buffer, *graph.buffer.buffer};
        const std::array<vk::DeviceSize, 2> arcs_offsets
        {
            graph.arc_points_offset,
            graph.arc_colors_offset
        };
        const std::array<vk::DeviceSize, 2> vertices_offsets
        {
            graph.vertice_points_offset,
            graph.vertice_colors_offset
        };
        constexpr uint32_t first_binding{0};
        constexpr uint32_t binding_count{graph_buffers.size()};

        constexpr uint32_t instance_count{1};
        constexpr uint32_t first_vertex{0};
        constexpr uint32_t first_instance{0};

        for (size_t i = 0; i < command_buffers.size(); ++i)
        {
            const vk::CommandBuffer& command_buffer = *command_buffers[i];

            begin(command_buffer, begin_info);

            render_pass_info.setFramebuffer(*framebuffers[i]);
            command_buffer.beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);

            command_buffer.bindVertexBuffers
            (
                first_binding,
                binding_count,
                graph_buffers.data(),
                arcs_offsets.data()
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

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, arcs_pipeline);

            command_buffer.draw
            (
                safe_cast<uint32_t>(graph.arc_points_count),
                instance_count,
                first_vertex,
                first_instance
            );

            command_buffer.bindVertexBuffers
            (
                first_binding,
                binding_count,
                graph_buffers.data(),
                vertices_offsets.data()
            );

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vertices_pipeline);

            command_buffer.draw
            (
                safe_cast<uint32_t>(graph.vertice_points_count)
                , instance_count
                , first_vertex
                , first_instance
            );

            // TODO: draw ui here

            command_buffer.endRenderPass();


            end(command_buffer);
        }

        return command_buffers;
    }
}
