module;

#include <vulkan/vulkan.hpp>

export module image_2d.shaders;

namespace ge::image2d
{
    export struct Shaders final
    {
        vk::UniqueShaderModule image_vertex;
        vk::UniqueShaderModule image_fragment;
    };
}
