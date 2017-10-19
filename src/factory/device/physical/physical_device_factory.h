#pragma once

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device::physical
{

    vk::PhysicalDevice create(const vk::Instance&, const vk::SurfaceKHR&);

} // namespace ge::impl::factory::device::physical
