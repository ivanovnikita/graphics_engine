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
    }

    std::tuple<vk::UniquePipeline, vk::UniquePipelineLayout, vk::UniqueRenderPass> create_graphics_pipeline
    (
        const vk::Device& logical_device
      , const vk::Format& format
      , const storage::Shaders& shaders_storage
      , const vk::Extent2D& extent
      , const vk::DescriptorSetLayout& descriptor_set_layout
    )
    {
        const std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_create_info = get_shader_stage_create_info
        (
            shaders_storage
            , ShaderName::line_2d_camera_Vertex
        );

        const vk::VertexInputBindingDescription binding_description = vertex_binding_description();
        const std::span<const vk::VertexInputAttributeDescription> attribute_description = vertex_attribute_descriptions();
        // TODO: use several bindings for coords and colors
        const auto vertex_input_info = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(1)
            .setPVertexBindingDescriptions(&binding_description)
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attribute_description.size()))
            .setPVertexAttributeDescriptions(attribute_description.data());

        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eTriangleList) // TODO: use line list
            .setPrimitiveRestartEnable(VK_FALSE);

        const auto viewport = vk::Viewport()
            .setX(0.0f)
            .setWidth(static_cast<float>(extent.width))

            // invert Y: https://www.saschawillems.de/blog/2019/03/29/flipping-the-vulkan-viewport/
            .setY(static_cast<float>(extent.height))
            .setHeight(-static_cast<float>(extent.height))

            .setMinDepth(0.0f)
            .setMaxDepth(1.0f);
        const vk::Rect2D scissor{vk::Offset2D{0, 0}, extent};
        const auto viewport_info = vk::PipelineViewportStateCreateInfo()
            .setViewportCount(1)
            .setPViewports(&viewport)
            .setScissorCount(1)
            .setPScissors(&scissor);

        const auto raster_info = vk::PipelineRasterizationStateCreateInfo()
            .setDepthClampEnable(VK_FALSE)
            .setRasterizerDiscardEnable(VK_FALSE)

            // TODO: only for line-pipeline
            .setPolygonMode(vk::PolygonMode::eLine)
            .setLineWidth(3.0f)

            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eCounterClockwise)
            .setDepthBiasEnable(VK_FALSE);

        const auto multisample_info = vk::PipelineMultisampleStateCreateInfo()
            .setSampleShadingEnable(VK_FALSE)
            .setRasterizationSamples(vk::SampleCountFlagBits::e1);

        const auto blend_attachment = vk::PipelineColorBlendAttachmentState()
            .setColorWriteMask
            (
                vk::ColorComponentFlagBits::eR
                | vk::ColorComponentFlagBits::eG
                | vk::ColorComponentFlagBits::eB
                | vk::ColorComponentFlagBits::eA
            )
            .setBlendEnable(VK_FALSE);

        const auto blend_state_info = vk::PipelineColorBlendStateCreateInfo()
            .setLogicOpEnable(VK_FALSE)
            .setLogicOp(vk::LogicOp::eCopy)
            .setAttachmentCount(1)
            .setPAttachments(&blend_attachment)
            .setBlendConstants({{0.0f, 0.0f, 0.0f, 0.0f}});

        const auto layout_info = vk::PipelineLayoutCreateInfo()
            .setSetLayoutCount(1)
            .setPSetLayouts(&descriptor_set_layout)
            .setPushConstantRangeCount(0);

        vk::UniquePipelineLayout layout = logical_device.createPipelineLayoutUnique(layout_info);

        vk::UniqueRenderPass render_pass = create_render_pass(logical_device, format);

        const auto pipeline_create_info = vk::GraphicsPipelineCreateInfo()
            .setStageCount(safe_cast<uint32_t>(shader_stage_create_info.size()))
            .setPStages(shader_stage_create_info.data())
            .setPVertexInputState(&vertex_input_info)
            .setPInputAssemblyState(&input_assembly_info)
            .setPViewportState(&viewport_info)
            .setPRasterizationState(&raster_info)
            .setPMultisampleState(&multisample_info)
            .setPColorBlendState(&blend_state_info)
            .setLayout(*layout)
            .setRenderPass(*render_pass)
            .setSubpass(0);

        vk::UniquePipeline pipeline = logical_device.createGraphicsPipelineUnique
        (
            vk::PipelineCache()
          , pipeline_create_info
        );

        return {std::move(pipeline), std::move(layout), std::move(render_pass)};
    }
}
