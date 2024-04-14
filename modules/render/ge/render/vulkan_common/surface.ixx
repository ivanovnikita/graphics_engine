module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.surface;

export import vulkan_common.surface_params;
export import logger;

namespace ge
{
    export struct SurfaceData final
    {
        static SurfaceData create_default(const SurfaceParams&, const vk::Instance&);

        vk::UniqueSurfaceKHR surface;
        vk::Extent2D extent;
        vk::ClearColorValue background_color;
    };
}
