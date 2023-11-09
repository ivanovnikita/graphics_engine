#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::image3d
{
    struct Shaders final
    {
        vk::UniqueShaderModule image_vertex;
        vk::UniqueShaderModule image_fragment;
    };
}
