#include "render/storage/shaders.h"
#include "render/factory/shader/tools.h"
#include "render/factory/shader/module.h"

namespace ge::storage
{
    Shaders::Shaders(const vk::Device& device, const fs::path& shaders_dir)
    {
        for (const auto& shader : fs::directory_iterator(shaders_dir))
        {
            shaders_.emplace_back
            (
                factory::shader_kind(shader)
              , factory::create_shader_module(device, shader)
            );
        }
    }

    std::vector<Shaders::Shader> Shaders::shaders() const
    {
        std::vector<Shaders::Shader> result;
        result.reserve(shaders_.size());

        for (const auto& [shader_kind, shader_module] : shaders_)
        {
            result.emplace_back(shader_kind, *shader_module);
        }

        return result;
    }
}
