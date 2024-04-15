module;

#include "ge/render/vulkan_common/vulkan_fwds.h"

export module tiles_2d.pipelines;

export import logger;
export import antialiasing;
export import vulkan_common.device;

namespace ge::tiles
{
    export vk::UniquePipeline create_triangles_pipeline
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

    export vk::UniquePipeline create_lines_pipeline
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
