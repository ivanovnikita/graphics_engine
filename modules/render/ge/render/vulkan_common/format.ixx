module;

#include "vulkan_fwds.h"

#include <span>

export module vulkan_common.format;

namespace ge
{
    export vk::Format find_supported_format
    (
        const vk::PhysicalDevice&,
        const std::span<const vk::Format> desired_formats,
        const vk::ImageTiling,
        const vk::FormatFeatureFlags&
    );
    export vk::Format find_depth_format(const vk::PhysicalDevice&);
    export bool has_stencil_component(vk::Format);
}
