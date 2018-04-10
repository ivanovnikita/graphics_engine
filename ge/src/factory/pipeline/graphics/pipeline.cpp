#include "pipeline.h"
#include "factory/shader/tools.h"
#include "factory/shader/module.h"

namespace ge::impl::factory::pipeline
{

    namespace
    {
        std::vector<vk::PipelineShaderStageCreateInfo> get_shader_stage_create_info(const storage::Shaders& shaders_storage)
        {
            const auto shaders = shaders_storage.shaders();

            std::vector<vk::PipelineShaderStageCreateInfo> result;
            result.reserve(shaders.size());

            for (const auto& [shader_kind, shader_module] : shaders)
            {
                result.emplace_back
                (
                    vk::PipelineShaderStageCreateInfo
                    {
                        vk::PipelineShaderStageCreateFlags()
                      , shader_kind
                      , shader_module
                      , "main"
                    }
                );
            }

            return result;
        }
    }

    vk::Pipeline create(const storage::Shaders& shaders_storage)
    {
        const auto shader_stage_create_info = get_shader_stage_create_info(shaders_storage);

        vk::GraphicsPipelineCreateInfo pipeline_create_info;
        return vk::Pipeline();
    }

}
