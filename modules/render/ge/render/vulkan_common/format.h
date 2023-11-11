#pragma once

#include <vulkan/vulkan.hpp>

#include <span>

namespace ge
{
    vk::Format find_supported_format
    (
        const vk::PhysicalDevice&,
        const std::span<const vk::Format> desired_formats,
        const vk::ImageTiling,
        const vk::FormatFeatureFlags&
    );
    vk::Format find_depth_format(const vk::PhysicalDevice&);
    bool has_stencil_component(vk::Format);
}
