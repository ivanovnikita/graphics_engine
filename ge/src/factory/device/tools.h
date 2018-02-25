#pragma once

#include <cstdint>
#include <optional>

namespace vk
{
    class PhysicalDevice;
    class SurfaceKHR;
}

namespace ge::impl::factory::device
{

    std::optional<uint32_t> get_suitable_queue_family_index(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface);

}
