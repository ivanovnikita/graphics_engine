#include "draw_image_commands.h"
#include "ge/render/vulkan_common/command_buffer.h"

#include "ge/common/safe_cast.hpp"

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

        const vk::ClearValue clear_color{background_color};

        vk::RenderPassBeginInfo render_pass_info = vk::RenderPassBeginInfo{}
            .setRenderPass(render_pass)
            .setRenderArea
            (
                vk::Rect2D{}
                    .setOffset(vk::Offset2D{0, 0})
                    .setExtent(extent)
            )
            .setClearValueCount(1)
            .setPClearValues(&clear_color);

        const std::array polygons_buffers{*polygons.buffer.buffer};
        const std::array<vk::DeviceSize, 1> offsets
        {
            polygons.offset
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

            command_buffer.bindVertexBuffers
            (
                first_binding,
                binding_count,
                polygons_buffers.data(),
                offsets.data()
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

            command_buffer.draw
            (
                safe_cast<uint32_t>(polygons.vertices_count),
                instance_count,
                first_vertex,
                first_instance
            );

            command_buffer.endRenderPass();


            end(command_buffer);
        }

        return command_buffers;
    }
}
