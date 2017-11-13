#pragma once

#include <cstdint>

namespace vk
{
    class PhysicalDevice;
    class SurfaceKHR;
}

namespace ge::impl::factory::device
{

    int32_t get_suitable_queue_family_index(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

} // namespace ge::impl::fatcory::device
