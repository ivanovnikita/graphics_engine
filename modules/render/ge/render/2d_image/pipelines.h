#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/antialiasing/antialiasing.h"
#include "ge/render/vulkan_common/device.h"

#include <vulkan/vulkan.hpp>

namespace ge::image2d
{
    vk::UniquePipeline create_image_pipeline
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
