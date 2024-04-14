module;

#include <vulkan/vulkan.hpp>

export module image_3d.shaders;

namespace ge::image3d
{
    export struct Shaders final
    {
        vk::UniqueShaderModule image_vertex;
        vk::UniqueShaderModule image_fragment;
    };
}
