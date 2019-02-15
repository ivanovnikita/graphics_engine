#include "ge/render/storage/shaders.h"
#include "ge/render/factory/shader/module.h"

#include "generated_shaders.h"

namespace ge::storage
{
    Shaders::Shaders(const vk::Device& device)
    {
        shaders_.emplace_back
        (
            vk::ShaderStageFlagBits::eVertex
          , factory::create_shader_module(device, triangle_Vertex)
        );
        shaders_.emplace_back
        (
            vk::ShaderStageFlagBits::eFragment
          , factory::create_shader_module(device, triangle_Fragment)
        );
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
