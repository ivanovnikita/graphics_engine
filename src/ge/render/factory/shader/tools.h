#pragma once

#include <vulkan/vulkan.hpp>

#include <experimental/filesystem>

namespace ge::factory
{
    namespace fs = std::experimental::filesystem;

    vk::ShaderStageFlagBits shader_kind(const fs::path& shader_code);
}
