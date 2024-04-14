module;

#include <vulkan/vulkan.hpp>

export module tiles_2d.shaders;

namespace ge::tiles
{
    export struct Shaders final
    {
        vk::UniqueShaderModule triangles_vertex;
        vk::UniqueShaderModule lines_vertex;
        vk::UniqueShaderModule fragment;
    };
}
