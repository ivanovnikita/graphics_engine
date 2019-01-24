#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace vk
{
    class PhysicalDevice;
    class SurfaceKHR;
}

namespace ge::factory
{
    std::optional<uint32_t> get_suitable_queue_family_index(const vk::PhysicalDevice&, const vk::SurfaceKHR&);
    std::vector<std::string> get_available_device_layers(const vk::PhysicalDevice&);
}
