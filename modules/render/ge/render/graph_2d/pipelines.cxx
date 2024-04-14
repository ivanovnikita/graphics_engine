module;

#include <vulkan/vulkan.hpp>

module graph_2d.pipelines;

import graph_2d.vertex_description;
import vulkan_common.pipeline;

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
            const Antialiasing& antialiasing,
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

            const bool depth_buffer_enabled = false;

            return create_pipeline_default
            (
                *device_data.logical_device,
                shader_stage_create_info,
                binding_descriptions,
                attribute_descriptions,
                input_assembly_info,
                raster_info,
                render_pass,
                extent,
                pipeline_layout,
                depth_buffer_enabled,
                antialiasing
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
        const Antialiasing& antialiasing,
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
            antialiasing,
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
        const Antialiasing& antialiasing,
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
            antialiasing,
            logger
        );
    }
}

