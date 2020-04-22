#include "ge/render/storage/shaders.h"
#include "ge/render/factory/shader/module.h"

namespace ge::storage
{
    Shaders::Shaders(const vk::Device& device)
    {
        const auto create_module = [this, &device] (const ShaderName shader_name)
        {
            shaders_.emplace(shader_name, factory::create_shader_module(device, get_shader(shader_name)));
        };

        create_module(ShaderName::point_2d_camera_Vertex);
        create_module(ShaderName::line_2d_camera_Vertex);
        create_module(ShaderName::simple_color_Fragment);
    }

    const vk::ShaderModule& Shaders::get(const ShaderName shader_name) const
    {
        return *shaders_.at(shader_name);
    }
}
