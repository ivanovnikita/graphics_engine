#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::storage
{
    class Shaders final
    {
    public:
        using ShaderModuleRef = std::reference_wrapper<const vk::ShaderModule>;
        using Shader = std::pair<vk::ShaderStageFlagBits, ShaderModuleRef>;

        Shaders() = default;
        explicit Shaders(const vk::Device& device);

        std::vector<Shader> shaders() const;

    private:
        std::vector<std::pair<vk::ShaderStageFlagBits, vk::UniqueShaderModule>> shaders_;
    };
}
