#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/device.h"

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    vk::UniquePipeline create_arcs_pipeline
    (
        const DeviceData&,
        const vk::RenderPass&,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D&,
        const vk::PipelineLayout&,
        const Logger&
    );

    vk::UniquePipeline create_vertices_pipeline
    (
        const DeviceData&,
        const vk::RenderPass&,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D&,
        const vk::PipelineLayout&,
        const Logger&
    );
}
