module;

#include <vulkan/vulkan.hpp>

export module image_3d.pipelines;

export import logger;
export import antialiasing;
export import vulkan_common.device;

namespace ge::image3d
{
    export vk::UniquePipeline create_image_pipeline
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
