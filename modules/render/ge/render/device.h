#pragma once

#include "ge/render/factory/options.h"
#include "ge/render/instance.h"

#include <vulkan/vulkan.hpp>

namespace ge
{
    struct PhysicalDeviceData final
    {
        vk::PhysicalDevice physical_device;
        vk::PhysicalDeviceProperties properties;
        std::optional<vk::PhysicalDeviceProperties2> properties2;
        vk::PhysicalDeviceFeatures features;
        std::optional<vk::PhysicalDeviceFeatures2> features2;
        vk::PhysicalDeviceMemoryProperties memory;
        // TODO: format properties?
    };

    struct DeviceData final
    {
        static void create_default
        (
            const factory::options::ValidationLayers&,
            const InstanceData&,
            const Logger&
        );

        PhysicalDeviceData physical_device_data;
        vk::UniqueDevice logical_device;
        uint32_t graphics_queue_family_index;
        uint32_t present_queue_family_index;
        vk::Queue graphics_queue;
        vk::Queue present_queue;
    };
}
