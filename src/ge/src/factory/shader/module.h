#pragma once

#include <vulkan/vulkan.hpp>

#include <experimental/filesystem>

namespace ge::impl::factory::shader::module
{

    namespace fs = std::experimental::filesystem;

    vk::UniqueShaderModule create
    (
        const vk::Device& logical_device
      , const fs::path& shader_code_file
    );

}

