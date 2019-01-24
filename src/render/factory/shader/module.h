#pragma once

#include <vulkan/vulkan.hpp>

#include <experimental/filesystem>

namespace ge::factory
{
    namespace fs = std::experimental::filesystem;

    vk::UniqueShaderModule create_shader_module
    (
        const vk::Device& logical_device
      , const fs::path& shader_code_file
    );
}

