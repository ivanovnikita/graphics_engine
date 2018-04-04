#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::pipeline::shader_stage
{

    vk::PipelineShaderStageCreateInfo create(const vk::ShaderModule&, vk::ShaderStageFlagBits);

}
