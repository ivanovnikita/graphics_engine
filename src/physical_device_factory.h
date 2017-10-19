#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::physical_device_factory
{

    vk::PhysicalDevice create(const vk::Instance&, const vk::SurfaceKHR& surface);

} // namespace ge::impl::physical_device_factory
