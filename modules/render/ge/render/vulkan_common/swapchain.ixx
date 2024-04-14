module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.swapchain;

export import vulkan_common.device;
export import vulkan_common.surface;

namespace ge
{
    export struct SwapchainData final
    {
        static SwapchainData create_default
        (
            const DeviceData&,
            SurfaceData&,
            const vk::Format desiredFormat
        );

        void reset();

        vk::UniqueSwapchainKHR swapchain;
        vk::Format format;
        vk::Extent2D extent;
        std::vector<vk::Image> images;
        std::vector<vk::UniqueImageView> image_views;
    };
}
