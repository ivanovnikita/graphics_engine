#include "pipeline.h"
#include "factory/shader/tools.h"
#include "factory/shader/module.h"

namespace ge::impl::factory::pipeline::graphics
{

    namespace
    {
        std::vector<vk::PipelineShaderStageCreateInfo> get_shader_stage_create_info(const storage::Shaders& shaders_storage)
        {
            const auto shaders = shaders_storage.shaders();

            std::vector<vk::PipelineShaderStageCreateInfo> result;
            result.reserve(shaders.size());

            for (const auto& [shader_kind, shader_module] : shaders)
            {
                result.emplace_back
                (
                    vk::PipelineShaderStageCreateInfo
                    {
                        vk::PipelineShaderStageCreateFlags()
                      , shader_kind
                      , shader_module
                      , "main"
                    }
                );
            }

            return result;
        }
    }

    vk::Pipeline create(const storage::Shaders& shaders_storage, const Window& window)
    {
        const auto shader_stage_create_info = get_shader_stage_create_info(shaders_storage);

        const vk::PipelineVertexInputStateCreateInfo vertex_input_info{};
        const auto input_assembly_info = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(VK_FALSE);

        const vk::Extent2D& extent = window.extent();
        const auto viewport = vk::Viewport()
            .setX(0.0f)
            .setY(0.0f)
            .setWidth(static_cast<float>(extent.width))
            .setHeight(static_cast<float>(extent.height))
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
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.0f)
            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eClockwise)
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

        const vk::PipelineLayoutCreateInfo layout_info{};

        vk::GraphicsPipelineCreateInfo pipeline_create_info;
        return vk::Pipeline();
    }

}
