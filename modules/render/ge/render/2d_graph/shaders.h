#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::graph
{
    struct Shaders final
    {
        vk::UniqueShaderModule lines_vertex;
        vk::UniqueShaderModule points_vertex;
        vk::UniqueShaderModule fragment;
    };
}
