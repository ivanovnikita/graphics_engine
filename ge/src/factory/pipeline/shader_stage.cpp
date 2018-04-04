#include "shader_stage.h"

namespace ge::impl::factory::pipeline::shader_stage
{

    vk::PipelineShaderStageCreateInfo create(const vk::ShaderModule& module, vk::ShaderStageFlagBits stage)
    {
        return vk::PipelineShaderStageCreateInfo
        {
            vk::PipelineShaderStageCreateFlags()
          , stage
          , module
          , "main"
        };
    }

}
