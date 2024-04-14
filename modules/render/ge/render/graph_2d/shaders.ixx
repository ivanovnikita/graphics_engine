module;

#include <vulkan/vulkan.hpp>

export module graph_2d.shaders;

namespace ge::graph
{
    export struct Shaders final
    {
        vk::UniqueShaderModule lines_vertex;
        vk::UniqueShaderModule points_vertex;
        vk::UniqueShaderModule fragment;
    };
}
