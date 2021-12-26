#pragma once

#include "ge/render/factory/options.h"
#include "ge/render/instance.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    // iterate over pNext: https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/chap3.html#fundamentals-validusage-pNext
    // grab VkPhysicalDeviceProperties and, if available, VkPhysicalDeviceProperties2

    struct DeviceData final
    {
        static void create_default
        (
            const factory::options::ValidationLayers&,
            const InstanceData&,
            const Logger&
        );

        vk::PhysicalDevice physical_device;
        vk::UniqueDevice logical_device;
        uint32_t graphics_queue_family_index;
        uint32_t present_queue_family_index;
        vk::Queue graphics_queue;
        vk::Queue present_queue;
    };
}
