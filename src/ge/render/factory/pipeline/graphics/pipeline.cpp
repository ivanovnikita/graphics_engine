#include "ge/render/factory/pipeline/graphics/pipeline.h"
#include "ge/render/factory/shader/module.h"
#include "ge/render/factory/pipeline/graphics/render_pass.h"
#include "ge/render/utils/safe_cast.hpp"
#include "ge/render/vertex_description.h"

namespace ge::factory
{
    namespace
    {
        std::vector<vk::PipelineShaderStageCreateInfo> get_shader_stage_create_info
        (
            const storage::Shaders& shaders_storage
            , const ShaderName vertex_shader_name
        )
        {
            std::vector<vk::PipelineShaderStageCreateInfo> result;
            result.reserve(2);

            const auto shader_create_info = [&shaders_storage] (const ShaderName shader_name)
            {
                const vk::ShaderModule& shader_module = shaders_storage.get(shader_name);
                return vk::PipelineShaderStageCreateInfo
                {
                    vk::PipelineShaderStageCreateFlags()
                  , get_shader_kind(shader_name)
                  , shader_module
                  , "main"
                };
            };

            result.emplace_back(shader_create_info(ShaderName::simple_color_Fragment));
            result.emplace_back(shader_create_info(vertex_shader_name));

            return result;
        }

        vk::Viewport default_viewport
        (
            const vk::Extent2D& extent
        )
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

        vk::Rect2D default_scissor(const vk::Extent2D& extent)
        {
            return vk::Rect2D{vk::Offset2D{0, 0}, extent};
        }

        vk::PipelineViewportStateCreateInfo viewport_create_info
        (
            const vk::Viewport& viewport
          , const vk::Rect2D& scissor
        )
        {
            return vk::PipelineViewportStateCreateInfo()
                .setViewportCount(1)
                .setPViewports(&viewport)
                .setScissorCount(1)
                .setPScissors(&scissor);
        }

        vk::PipelineMultisampleStateCreateInfo multisample_create_info()
        {
            return vk::PipelineMultisampleStateCreateInfo()
                .setSampleShadingEnable(VK_FALSE)
                .setRasterizationSamples(vk::SampleCountFlagBits::e1);
        }

        vk::PipelineColorBlendAttachmentState default_blend_attachment()
        {
            return vk::PipelineColorBlendAttachmentState()
                .setColorWriteMask
                (
                    vk::ColorComponentFlagBits::eR
                    | vk::ColorComponentFlagBits::eG
                    | vk::ColorComponentFlagBits::eB
                    | vk::ColorComponentFlagBits::eA
                )
                .setBlendEnable(VK_FALSE);
        }

        vk::PipelineColorBlendStateCreateInfo blend_create_info
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
    }

    vk::UniquePipelineLayout camera_2d_pipeline_layout
    (
        const vk::Device& logical_device
      , const vk::DescriptorSetLayout& descriptor_set_layout
    )
    {
        const auto layout_info = vk::PipelineLayoutCreateInfo()
            .setSetLayoutCount(1)
            .setPSetLayouts(&descriptor_set_layout)
            .setPushConstantRangeCount(0);

        return logical_device.createPipelineLayoutUnique(layout_info);
    }

    vk::UniquePipeline graph_arcs_pipeline
    (
        const vk::Device& logical_device
      , const vk::RenderPass& render_pass
      , const storage::Shaders& shaders_storage
      , const vk::Extent2D& extent
      , const vk::PipelineLayout& pipeline_layout
    )
    {
        const std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_info = get_shader_stage_create_info
        (
            shaders_storage
            , ShaderName::line_2d_camera_Vertex
        );

        const std::span<const vk::VertexInputBindingDescription> binding_description = vertex_binding_description();
        const std::span<const vk::VertexInputAttributeDescription> attribute_description = vertex_attribute_descriptions();
        const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(static_cast<uint32_t>(binding_description.size()))
            .setPVertexBindingDescriptions(binding_description.data())
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_description.size()))
            .setPVertexAttributeDescriptions(attribute_description.data());

        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eLineList)
            .setPrimitiveRestartEnable(VK_FALSE);

        const vk::Viewport viewport = default_viewport(extent);
        const vk::Rect2D scissor = default_scissor(extent);
        const vk::PipelineViewportStateCreateInfo viewport_info = viewport_create_info(viewport, scissor);

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            // TODO: only for line-pipeline
            .setPolygonMode(vk::PolygonMode::eLine)
            .setLineWidth(3.0f)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        const vk::PipelineMultisampleStateCreateInfo multisample_info = multisample_create_info();

        const vk::PipelineColorBlendAttachmentState blend_attachment = default_blend_attachment();
        const vk::PipelineColorBlendStateCreateInfo blend_state_info = blend_create_info(blend_attachment);

        const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
            .setStageCount(safe_cast<uint32_t>(shader_stage_create_info.size()))
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

        vk::UniquePipeline pipeline = logical_device.createGraphicsPipelineUnique
        (
            vk::PipelineCache()
          , pipeline_create_info
        );

        return pipeline;
    }

    vk::UniquePipeline graph_vertices_pipeline
    (
        const vk::Device& logical_device
      , const vk::RenderPass& render_pass
      , const storage::Shaders& shaders_storage
      , const vk::Extent2D& extent
      , const vk::PipelineLayout& pipeline_layout
    )
    {
        const std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_info = get_shader_stage_create_info
        (
            shaders_storage
            , ShaderName::point_2d_camera_Vertex
        );

        const std::span<const vk::VertexInputBindingDescription> binding_description = vertex_binding_description();
        const std::span<const vk::VertexInputAttributeDescription> attribute_description = vertex_attribute_descriptions();
        const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(static_cast<uint32_t>(binding_description.size()))
            .setPVertexBindingDescriptions(binding_description.data())
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_description.size()))
            .setPVertexAttributeDescriptions(attribute_description.data());

        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::ePointList)
            .setPrimitiveRestartEnable(VK_FALSE);

        const vk::Viewport viewport = default_viewport(extent);
        const vk::Rect2D scissor = default_scissor(extent);
        const vk::PipelineViewportStateCreateInfo viewport_info = viewport_create_info(viewport, scissor);

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            .setPolygonMode(vk::PolygonMode::ePoint)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        const vk::PipelineMultisampleStateCreateInfo multisample_info = multisample_create_info();

        const vk::PipelineColorBlendAttachmentState blend_attachment = default_blend_attachment();
        const vk::PipelineColorBlendStateCreateInfo blend_state_info = blend_create_info(blend_attachment);

        const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
            .setStageCount(safe_cast<uint32_t>(shader_stage_create_info.size()))
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

        vk::UniquePipeline pipeline = logical_device.createGraphicsPipelineUnique
        (
            vk::PipelineCache()
          , pipeline_create_info
        );

        return pipeline;
    }

    vk::UniquePipeline polygon_pipeline
    (
        const vk::Device& logical_device,
        const vk::RenderPass& render_pass,
        const storage::Shaders& shaders_storage,
        const vk::Extent2D& extent,
        const vk::PipelineLayout& pipeline_layout
    )
    {
        const std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_info = get_shader_stage_create_info
        (
            shaders_storage
            , ShaderName::polygon_2d_camera_Vertex
        );

        const std::span<const vk::VertexInputBindingDescription> binding_description = vertex_binding_description();
        const std::span<const vk::VertexInputAttributeDescription> attribute_description = vertex_attribute_descriptions();
        const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(static_cast<uint32_t>(binding_description.size()))
            .setPVertexBindingDescriptions(binding_description.data())
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_description.size()))
            .setPVertexAttributeDescriptions(attribute_description.data());

        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(VK_FALSE);

        const vk::Viewport viewport = default_viewport(extent);
        const vk::Rect2D scissor = default_scissor(extent);
        const vk::PipelineViewportStateCreateInfo viewport_info = viewport_create_info(viewport, scissor);

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.f)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        const vk::PipelineMultisampleStateCreateInfo multisample_info = multisample_create_info();

        const vk::PipelineColorBlendAttachmentState blend_attachment = default_blend_attachment();
        const vk::PipelineColorBlendStateCreateInfo blend_state_info = blend_create_info(blend_attachment);

        const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
            .setStageCount(safe_cast<uint32_t>(shader_stage_create_info.size()))
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

        vk::UniquePipeline pipeline = logical_device.createGraphicsPipelineUnique
        (
            vk::PipelineCache()
          , pipeline_create_info
        );

        return pipeline;
    }

    vk::UniquePipeline lines_pipeline
    (
        const vk::Device& logical_device,
        const vk::RenderPass& render_pass,
        const storage::Shaders& shaders_storage,
        const vk::Extent2D& extent,
        const vk::PipelineLayout& pipeline_layout
    )
    {
        const std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_info = get_shader_stage_create_info
        (
            shaders_storage
            , ShaderName::line_2d_camera_Vertex
        );

        const std::span<const vk::VertexInputBindingDescription> binding_description = vertex_binding_description();
        const std::span<const vk::VertexInputAttributeDescription> attribute_description = vertex_attribute_descriptions();
        const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(static_cast<uint32_t>(binding_description.size()))
            .setPVertexBindingDescriptions(binding_description.data())
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_description.size()))
            .setPVertexAttributeDescriptions(attribute_description.data());

        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eLineList)
            .setPrimitiveRestartEnable(VK_FALSE);

        const vk::Viewport viewport = default_viewport(extent);
        const vk::Rect2D scissor = default_scissor(extent);
        const vk::PipelineViewportStateCreateInfo viewport_info = viewport_create_info(viewport, scissor);

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            // TODO: only for line-pipeline
            .setPolygonMode(vk::PolygonMode::eLine)
            .setLineWidth(2.0f)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        const vk::PipelineMultisampleStateCreateInfo multisample_info = multisample_create_info();

        const vk::PipelineColorBlendAttachmentState blend_attachment = default_blend_attachment();
        const vk::PipelineColorBlendStateCreateInfo blend_state_info = blend_create_info(blend_attachment);

        const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
            .setStageCount(safe_cast<uint32_t>(shader_stage_create_info.size()))
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

        vk::UniquePipeline pipeline = logical_device.createGraphicsPipelineUnique
        (
            vk::PipelineCache()
          , pipeline_create_info
        );

        return pipeline;
    }
}
