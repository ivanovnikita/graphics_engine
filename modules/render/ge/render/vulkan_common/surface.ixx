module;

#include "exception_macro.h"

#include <vulkan/vulkan.hpp>

export module vulkan_common.surface;

export import vulkan_common.surface_params;
export import logger;

import vulkan_common.exception;

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

module : private;

namespace ge
{
    namespace
    {
        vk::UniqueSurfaceKHR create_surface
        (
            const XcbSurface& xcb,
            const vk::Instance& instance
        )
        {
            const vk::XcbSurfaceCreateInfoKHR create_info
            {
                vk::XcbSurfaceCreateFlagsKHR(),
                &xcb.connection.get(),
                xcb.window
            };

            vk::SurfaceKHR surface;
            const vk::Result result = instance.createXcbSurfaceKHR
            (
                &create_info,
                nullptr,
                &surface
            );
            switch (result)
            {
            case vk::Result::eSuccess:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(result, "Surface creation failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result, "Surface creation failed");
            }
            }

            return vk::UniqueSurfaceKHR{surface, {instance}};
        }
    }

    SurfaceData SurfaceData::create_default
    (
        const SurfaceParams& params,
        const vk::Instance& instance
    )
    {
        vk::UniqueSurfaceKHR surface = std::visit
        (
            [&instance] (const auto& p)
            {
                return create_surface(p, instance);
            },
            params.surface
        );

        return SurfaceData
        {
            std::move(surface),

            vk::Extent2D{}
                .setWidth(params.width)
                .setHeight(params.height),

            // TODO: this conversion may be incorrect, use linear to sRGB colorspace conversion
            std::array<float, 4>
            {
                params.background_color[0] / 255.f
              , params.background_color[1] / 255.f
              , params.background_color[2] / 255.f
              , params.background_color[3] / 255.f
            }
        };
    }
}
