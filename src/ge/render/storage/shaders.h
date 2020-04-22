#pragma once

#include "generated_shaders.h"

#include <vulkan/vulkan.hpp>

#include <unordered_map>

namespace ge::storage
{
    class Shaders final
    {
    public:
        Shaders() = default;
        explicit Shaders(const vk::Device& device);

        const vk::ShaderModule& get(ShaderName) const;

    private:
        std::unordered_map<ShaderName, vk::UniqueShaderModule> shaders_;
    };
}
