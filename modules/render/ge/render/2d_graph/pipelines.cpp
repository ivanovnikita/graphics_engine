#include "pipelines.h"
#include "vertex_description.h"
#include "ge/render/vulkan_common/create_pipeline.h"

namespace ge::graph
{
    namespace
    {
        std::array<vk::PipelineShaderStageCreateInfo, 2> get_shader_stage_create_info
        (
            const vk::ShaderModule& vertex,
            const vk::ShaderModule& fragment
        )
        {
            return
            {
                vk::PipelineShaderStageCreateInfo
                {
                    vk::PipelineShaderStageCreateFlags{},
                    vk::ShaderStageFlagBits::eVertex,
                    vertex,
                    "main"
                },
                vk::PipelineShaderStageCreateInfo
                {
                    vk::PipelineShaderStageCreateFlags{},
                    vk::ShaderStageFlagBits::eFragment,
                    fragment,
                    "main"
                }
            };
        }

        vk::Viewport get_viewport(const vk::Extent2D& extent)
        {
            return vk::Viewport()
                .setX(0.0f)
                .setWidth(static_cast<float>(extent.width))

                // invert Y: https://www.saschawillems.de/blog/2019/03/29/flipping-the-vulkan-viewport/
                .setY(static_cast<float>(extent.height))
                .setHeight(-static_cast<float>(extent.height))

                .setMinDepth(0.0f)
                .setMaxDepth(1.0f);
        }

        vk::Rect2D get_scissor(const vk::Extent2D& extent)
        {
            return vk::Rect2D{vk::Offset2D{0, 0}, extent};
        }

        vk::PipelineViewportStateCreateInfo get_viewport_create_info
        (
            const vk::Viewport& viewport,
            const vk::Rect2D& scissor
        )
        {
            return vk::PipelineViewportStateCreateInfo()
                .setViewportCount(1)
                .setPViewports(&viewport)
                .setScissorCount(1)
                .setPScissors(&scissor);
        }

        vk::PipelineMultisampleStateCreateInfo get_multisample_create_info()
        {
            return vk::PipelineMultisampleStateCreateInfo()
                .setSampleShadingEnable(VK_FALSE)
                .setRasterizationSamples(vk::SampleCountFlagBits::e1);
        }

        vk::PipelineColorBlendAttachmentState get_blend_attachment()
        {
            return vk::PipelineColorBlendAttachmentState()
                .setColorWriteMask
                (
                    vk::ColorComponentFlagBits::eR |
                    vk::ColorComponentFlagBits::eG |
                    vk::ColorComponentFlagBits::eB |
                    vk::ColorComponentFlagBits::eA
                )
                .setBlendEnable(VK_FALSE);
        }

        vk::PipelineColorBlendStateCreateInfo get_blend_create_info
        (
            const vk::PipelineColorBlendAttachmentState& blend_attachment
        )
        {
            return vk::PipelineColorBlendStateCreateInfo()
                .setLogicOpEnable(VK_FALSE)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachmentCount(1)
                .setPAttachments(&blend_attachment)
                .setBlendConstants({{0.0f, 0.0f, 0.0f, 0.0f}});
        }

        vk::UniquePipeline new_pipeline
        (
            const vk::Device& device,
            const std::span<const vk::PipelineShaderStageCreateInfo> shader_stage_create_info,
            const std::span<const vk::VertexInputBindingDescription> binding_descriptions,
            const std::span<const vk::VertexInputAttributeDescription> attribute_descriptions,
            const vk::PipelineInputAssemblyStateCreateInfo input_assembly_info,
            const vk::PipelineRasterizationStateCreateInfo raster_info,
            const vk::RenderPass& render_pass,
            const vk::Extent2D& extent,
            const vk::PipelineLayout& pipeline_layout
        )
        {
            const vk::Viewport viewport = get_viewport(extent);
            const vk::Rect2D scissor = get_scissor(extent);
            const vk::PipelineViewportStateCreateInfo viewport_info = get_viewport_create_info(viewport, scissor);

            const vk::PipelineMultisampleStateCreateInfo multisample_info = get_multisample_create_info();

            const vk::PipelineColorBlendAttachmentState blend_attachment = get_blend_attachment();
            const vk::PipelineColorBlendStateCreateInfo blend_state_info = get_blend_create_info(blend_attachment);

            const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
                .setVertexBindingDescriptionCount(static_cast<uint32_t>(binding_descriptions.size()))
                .setPVertexBindingDescriptions(binding_descriptions.data())
                .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_descriptions.size()))
                .setPVertexAttributeDescriptions(attribute_descriptions.data());

            const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
                .setStageCount(static_cast<uint32_t>(shader_stage_create_info.size()))
                .setPStages(shader_stage_create_info.data())
                .setPVertexInputState(&vertex_input_info)
                .setPInputAssemblyState(&input_assembly_info)
                .setPViewportState(&viewport_info)
                .setPRasterizationState(&raster_info)
                .setPMultisampleState(&multisample_info)
                .setPColorBlendState(&blend_state_info)
                .setLayout(pipeline_layout)
                .setRenderPass(render_pass)
                .setSubpass(0);

            // TODO: Создать общий конвеер и от него создать частные дочерние конвееры.
            // Cоздание производного конвейера не требует больших затрат, поскольку большинство функций берется
            // из родительского конвейера. Также переключение между дочерними конвейерами одного родителя осуществляется
            // намного быстрее.

            return create_pipeline(device, pipeline_create_info);
        }

        vk::UniquePipeline new_graph_pipeline
        (
            const DeviceData& device_data,
            const vk::ShaderModule& vertex,
            const vk::ShaderModule& fragment,
            const vk::PipelineInputAssemblyStateCreateInfo input_assembly_info,
            const vk::PipelineRasterizationStateCreateInfo raster_info,
            const vk::RenderPass& render_pass,
            const vk::Extent2D& extent,
            const vk::PipelineLayout& pipeline_layout,
            const Logger& logger
        )
        {
            const std::array shader_stage_create_info = get_shader_stage_create_info
            (
                vertex,
                fragment
            );

            const std::array binding_descriptions = get_vertex_binding_descriptions
            (
                device_data.physical_device_data.properties.limits,
                logger
            );
            const std::array attribute_descriptions = get_vertex_attribute_descriptions
            (
                device_data.physical_device_data.properties.limits,
                logger
            );

            return new_pipeline
            (
                *device_data.logical_device,
                shader_stage_create_info,
                binding_descriptions,
                attribute_descriptions,
                input_assembly_info,
                raster_info,
                render_pass,
                extent,
                pipeline_layout
            );
        }
    }

    vk::UniquePipeline create_arcs_pipeline
    (
        const DeviceData& device_data,
        const vk::RenderPass& render_pass,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D& extent,
        const vk::PipelineLayout& pipeline_layout,
        const Logger& logger
    )
    {
        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eLineList)
            .setPrimitiveRestartEnable(VK_FALSE);

        assert(device_data.enabled_features.test(DeviceFeatures::WideLines)); // for LineWidth
        assert(device_data.enabled_features.test(DeviceFeatures::FillModeNonSolid)); // for vk::PolygonMode::eLine

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            // TODO: only for line-pipeline
            .setPolygonMode(vk::PolygonMode::eLine)
            .setLineWidth(3.0f)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        return new_graph_pipeline
        (
            device_data,
            vertex,
            fragment,
            input_assembly_info,
            raster_info,
            render_pass,
            extent,
            pipeline_layout,
            logger
        );
    }

    vk::UniquePipeline create_vertices_pipeline
    (
        const DeviceData& device_data,
        const vk::RenderPass& render_pass,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D& extent,
        const vk::PipelineLayout& pipeline_layout,
        const Logger& logger
    )
    {
        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::ePointList)
            .setPrimitiveRestartEnable(VK_FALSE);

        assert(device_data.enabled_features.test(DeviceFeatures::FillModeNonSolid)); // for vk::PolygonMode::ePoint

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            .setPolygonMode(vk::PolygonMode::ePoint)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        return new_graph_pipeline
        (
            device_data,
            vertex,
            fragment,
            input_assembly_info,
            raster_info,
            render_pass,
            extent,
            pipeline_layout,
            logger
        );
    }
}

