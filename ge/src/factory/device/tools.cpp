#include "factory/device/tools.h"

#include <vulkan/vulkan.hpp>

namespace ge::impl::factory::device
{

    std::optional<uint32_t> get_suitable_queue_family_index(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        const auto queue_family_properties = device.getQueueFamilyProperties();
        const auto required_queue_family_flags = vk::QueueFlagBits::eGraphics;

        for (uint32_t i = 0; i < queue_family_properties.size(); ++i)
        {
            if
            (
                queue_family_properties[i].queueCount > 0
             && queue_family_properties[i].queueFlags & required_queue_family_flags
             && device.getSurfaceSupportKHR(i, surface) == VK_TRUE
            )
            {
                return i;
            }
        }
        return {};
    }

}

