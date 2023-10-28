#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::tiles
{
    struct Shaders final
    {
        vk::UniqueShaderModule triangles_vertex;
        vk::UniqueShaderModule lines_vertex;
        vk::UniqueShaderModule fragment;
    };
}
