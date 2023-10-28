#include "create_pipeline_layout.h"
#include "exception.h"

namespace ge
{
    vk::UniquePipelineLayout create_pipeline_layout
    (
        const vk::Device& device,
        const vk::PipelineLayoutCreateInfo& create_info
    )
    {
        vk::PipelineLayout layout;
        const vk::Result result = device.createPipelineLayout(&create_info, nullptr, &layout);
        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
            GE_THROW_EXPECTED_RESULT(result, "Pipeline layout creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Pipeline layout creation failed");
        }
        }

        return vk::UniquePipelineLayout
        {
            std::move(layout),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
