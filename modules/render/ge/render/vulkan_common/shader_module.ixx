module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

#include <span>

export module vulkan_common.shader_module;

import vulkan_common.exception;

namespace ge
{
    export vk::UniqueShaderModule create_shader_module
    (
        const vk::Device&,
        std::span<const uint32_t> spirv_code
    );
}

module : private;

namespace ge
{
    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device& device,
        std::span<const uint32_t> spirv_code
    )
    {
        const vk::ShaderModuleCreateInfo create_info
        {
            vk::ShaderModuleCreateFlags{},
            spirv_code.size() * sizeof(uint32_t),
            spirv_code.data()
        };

        vk::ShaderModule shader_module;
        const vk::Result result = device.createShaderModule
        (
            &create_info,
            nullptr,
            &shader_module
        );

        switch (result)
        {
        case vk::Result::eSuccess:
            break;
        case vk::Result::eErrorOutOfHostMemory:
        case vk::Result::eErrorOutOfDeviceMemory:
        case vk::Result::eErrorInvalidShaderNV:
            GE_THROW_EXPECTED_RESULT(result, "Shader module creation failed");
        default:
        {
            GE_THROW_UNEXPECTED_RESULT(result, "Shader module creation failed");
        }
        }

        return vk::UniqueShaderModule
        {
            std::move(shader_module),
            vk::ObjectDestroy<vk::Device, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>{device}
        };
    }
}
