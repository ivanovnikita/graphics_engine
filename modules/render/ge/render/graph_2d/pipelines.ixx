module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module graph_2d.pipelines;

export import logger;
export import antialiasing;
export import vulkan_common.device;

namespace ge::graph
{
    export vk::UniquePipeline create_arcs_pipeline
    (
        const DeviceData&,
        const vk::RenderPass&,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D&,
        const vk::PipelineLayout&,
        const Antialiasing&,
        const Logger&
    );

    export vk::UniquePipeline create_vertices_pipeline
    (
        const DeviceData&,
        const vk::RenderPass&,
        const vk::ShaderModule& vertex,
        const vk::ShaderModule& fragment,
        const vk::Extent2D&,
        const vk::PipelineLayout&,
        const Antialiasing&,
        const Logger&
    );
}
