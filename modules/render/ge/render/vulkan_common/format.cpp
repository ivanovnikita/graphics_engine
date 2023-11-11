#include "format.h"

#include "ge/common/exception.h"

namespace ge
{
    vk::Format find_supported_format
    (
        const vk::PhysicalDevice& device,
        const std::span<const vk::Format> desired_formats,
        const vk::ImageTiling tiling,
        const vk::FormatFeatureFlags& features
    )
    {
        for (const vk::Format desired_format : desired_formats)
        {
            const vk::FormatProperties properties = device.getFormatProperties(desired_format);
            switch (tiling)
            {
            case vk::ImageTiling::eLinear:
            {
                if ((properties.linearTilingFeatures & features) == features)
                {
                    return desired_format;
                }
                break;
            }
            case vk::ImageTiling::eOptimal:
            {
                if ((properties.optimalTilingFeatures & features) == features)
                {
                    return desired_format;
                }
                break;
            }
            case vk::ImageTiling::eDrmFormatModifierEXT:
            {
                break;
            }
            }
        }

        GE_THROW_EXPECTED_ERROR("Desired format is unsupported");
    }

    vk::Format find_depth_format(const vk::PhysicalDevice& device)
    {
        constexpr std::array desired_formats
        {
            vk::Format::eD32Sfloat,
            vk::Format::eD32SfloatS8Uint,
            vk::Format::eD24UnormS8Uint
        };
        return find_supported_format
        (
            device,
            desired_formats,
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment
        );
    }

    bool has_stencil_component(const vk::Format format)
    {
        return format == vk::Format::eD32SfloatS8Uint or format == vk::Format::eD24UnormS8Uint;
    }
}
