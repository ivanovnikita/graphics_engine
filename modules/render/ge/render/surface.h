#pragma once

#include "ge/common/logger.hpp"
#include "ge/render/surface_params.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct SurfaceData final
    {
        static SurfaceData create_default(const SurfaceParams&, const vk::Instance&);

        vk::UniqueSurfaceKHR surface;
        vk::Extent2D extent;
        vk::ClearColorValue background_color;
    };
}
