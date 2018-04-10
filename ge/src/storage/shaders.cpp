#include "shaders.h"
#include "factory/shader/tools.h"
#include "factory/shader/module.h"

namespace ge::impl::storage
{

    Shaders::Shaders(const vk::Device& device, const fs::path& shaders_dir)
    {
        for (const auto& shader : fs::directory_iterator(shaders_dir))
        {
            shaders_.emplace_back
            (
                factory::shader::tools::shader_kind(shader)
              , factory::shader::module::create(device, shader)
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
