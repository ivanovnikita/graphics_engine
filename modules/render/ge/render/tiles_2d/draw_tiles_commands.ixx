module;

#include <vulkan/vulkan.hpp>

#include <span>

export module tiles_2d.draw_tiles_commands;

export import tiles_2d.polygons_in_device_memory;

import vulkan_common.command_buffer;
import safe_cast;

namespace ge::tiles
{
    export std::vector<vk::UniqueCommandBuffer> draw_tiles_commands
    (
        const vk::Device&,
        const vk::CommandPool&,
        const std::span<const vk::UniqueFramebuffer>&,
        const vk::RenderPass&,
        const vk::Extent2D&,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& triangles_pipeline,
        const vk::Pipeline& lines_pipeline,
        const vk::PipelineLayout&,
        const std::span<const vk::UniqueDescriptorSet>,
        const PolygonsInDeviceMemory&
    );
}

module : private;

namespace ge::tiles
{
    std::vector<vk::UniqueCommandBuffer> draw_tiles_commands
    (
        const vk::Device& device,
        const vk::CommandPool& command_pool,
        const std::span<const vk::UniqueFramebuffer>& framebuffers,
        const vk::RenderPass& render_pass,
        const vk::Extent2D& extent,
        const vk::ClearColorValue& background_color,
        const vk::Pipeline& triangles_pipeline,
        const vk::Pipeline& lines_pipeline,
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

        const std::array polygons_buffers{*polygons.buffer.buffer, *polygons.buffer.buffer};
        const std::array<vk::DeviceSize, 2> triangle_offsets
        {
            polygons.triangle_points_offset,
            polygons.triangle_colors_offset
        };
        const std::array<vk::DeviceSize, 2> line_offsets
        {
            polygons.line_points_offset,
            polygons.line_colors_offset
        };
        constexpr uint32_t first_binding{0};
        constexpr uint32_t binding_count{polygons_buffers.size()};

        constexpr uint32_t instance_count{1};
        constexpr uint32_t first_vertex{0};
        constexpr uint32_t first_instance{0};

        for (size_t i = 0; i < command_buffers.size(); ++i)
        {
            const vk::CommandBuffer& command_buffer = *command_buffers[i];

            begin(command_buffer, begin_info);

            render_pass_info.setFramebuffer(*framebuffers[i]);
            command_buffer.beginRenderPass(&render_pass_info, vk::SubpassContents::eInline);

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

            command_buffer.bindVertexBuffers
            (
                first_binding,
                binding_count,
                polygons_buffers.data(),
                triangle_offsets.data()
            );

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, triangles_pipeline);

            command_buffer.draw
            (
                safe_cast<uint32_t>(polygons.triangle_points_count),
                instance_count,
                first_vertex,
                first_instance
            );

            command_buffer.bindVertexBuffers
            (
                first_binding,
                binding_count,
                polygons_buffers.data(),
                line_offsets.data()
            );

            command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, lines_pipeline);

            command_buffer.draw
            (
                safe_cast<uint32_t>(polygons.line_points_count)
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
