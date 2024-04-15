module;

#include <vulkan/vulkan.hpp>

export module vulkan_common.device;

export import vulkan_common.device_layers;
export import vulkan_common.device_extensions;
export import vulkan_common.device_features;

import logger;
import vulkan_common.instance_fwd;

namespace ge
{
    export struct PhysicalDeviceData final
    {
        vk::PhysicalDevice physical_device;
        vk::PhysicalDeviceProperties properties;
        std::optional<vk::PhysicalDeviceProperties2> properties2;
        vk::PhysicalDeviceFeatures features;
        std::optional<vk::PhysicalDeviceFeatures2> features2;
        vk::PhysicalDeviceMemoryProperties memory;
        // TODO: format properties?
        std::vector<vk::QueueFamilyProperties> queue_properties;
        // TODO: vk::QueueFamilyProperties2 ?
        std::optional<size_t> graphics_queue_index;
        std::optional<size_t> present_queue_index;
        std::optional<size_t> compute_queue_index;
        std::optional<size_t> transfer_queue_index;
    };

    // TODO: add enum for required extensions and layers; save enabled extensions and layers
    export struct DeviceData final
    {
        static DeviceData create_default
        (
            DeviceLayerFlags,
            DeviceExtensionFlags,
            DeviceFeaturesFlags,
            const InstanceData&,
            const vk::SurfaceKHR&,
            const Logger&
        );

        PhysicalDeviceData physical_device_data;
        vk::UniqueDevice logical_device;
        DeviceLayerFlags enabled_layers;
        DeviceExtensionFlags enabled_extensions;
        DeviceFeaturesFlags enabled_features;
        uint32_t graphics_queue_family_index;
        uint32_t present_queue_family_index;
        vk::Queue graphics_queue;
        vk::Queue present_queue;
    };

    export void wait_idle(const vk::Device&);
}
