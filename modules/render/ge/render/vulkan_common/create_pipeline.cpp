#include "create_pipeline.h"
#include "exception.h"

namespace ge
{
    vk::UniquePipeline create_pipeline
    (
        const vk::Device& device,
        const vk::GraphicsPipelineCreateInfo& create_info
    )
    {
        vk::Pipeline pipeline;
        const vk::Result result = device.createGraphicsPipelines
        (
            vk::PipelineCache{},
            1,
            &create_info,
            nullptr,
            &pipeline
        );

        switch (result)
        {
        case vk::Result::eSuccess:
        case vk::Result::ePipelineCompileRequiredEXT:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidShaderNV:
            GE_THROW_EXPECTED_RESULT(result, "Pipeline creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Pipeline creation failed");
        }
        }

        return vk::UniquePipeline
        {
            std::move(pipeline),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
