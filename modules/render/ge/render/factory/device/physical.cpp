#include "ge/common/safe_cast.hpp"
#include "ge/render/factory/device/physical.h"
#include "ge/render/factory/device/requirements.h"
#include "ge/render/factory/device/tools.h"
#include "ge/render/factory/tools.hpp"
#include "ge/render/exception.h"

namespace ge::factory
{
    namespace
    {
        std::vector<std::string> get_available_device_extensions(const vk::PhysicalDevice& device)
        {
            return extensions_names(device.enumerateDeviceExtensionProperties());
        }

        uint32_t get_queue_family_index(const vk::PhysicalDevice& device, vk::QueueFlagBits required_bit)
        {
            const auto queue_family_properties = device.getQueueFamilyProperties();

            if (required_bit == vk::QueueFlagBits::eCompute)
            {
                for (uint32_t i = 0; i < safe_cast<uint32_t>(queue_family_properties.size()); ++i)
                {
                    if
                    (
                        queue_family_properties[i].queueFlags & required_bit
                     && queue_family_properties[i].queueCount > 0
                     && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics)
                    )
                    {
                        return i;
                    }
                }
            }

            if (required_bit == vk::QueueFlagBits::eTransfer)
            {
                for (uint32_t i = 0; i < safe_cast<uint32_t>(queue_family_properties.size()); ++i)
                {
                    if
                    (
                        queue_family_properties[i].queueFlags & required_bit
                     && queue_family_properties[i].queueCount > 0
                     && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eGraphics)
                     && !(queue_family_properties[i].queueFlags & vk::QueueFlagBits::eCompute)
                    )
                    {
                        return i;
                    }
                }
            }

            for (uint32_t i = 0; i < safe_cast<uint32_t>(queue_family_properties.size()); ++i)
            {
                if
                (
                    queue_family_properties[i].queueFlags & required_bit
                 && queue_family_properties[i].queueCount > 0
                )
                {
                    return i;
                }
            }

            GE_THROW(device_capabilities_error, "There is no required queue family.");
        }

        uint32_t get_presentation_queue_family_index(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
        {
            all_required_are_available
            (
                get_required_device_extensions()
              , get_available_device_extensions(device)
            );

            const auto queue_family_properties = device.getQueueFamilyProperties();

            for (uint32_t i = 0; i < safe_cast<uint32_t>(queue_family_properties.size()); ++i)
            {
                if
                (
                    device.getSurfaceSupportKHR(i, surface) == VK_TRUE
                 && queue_family_properties[i].queueCount > 0
                )
                {
                    return i;
                }
            }

            GE_THROW(device_capabilities_error, "There is no required queue family.");
        }

        void sort_by_type(std::vector<vk::PhysicalDevice>& devices)
        {
            auto get_type_priority = [] (vk::PhysicalDeviceType type)
            {
                switch (type)
                {
                case vk::PhysicalDeviceType::eDiscreteGpu: return 0;
                case vk::PhysicalDeviceType::eIntegratedGpu: return 1;
                case vk::PhysicalDeviceType::eCpu: return 2;
                case vk::PhysicalDeviceType::eVirtualGpu:
                case vk::PhysicalDeviceType::eOther:
                    return 3;
                }

                GE_THROW(device_capabilities_error, "Unknown physical device type");
            };
            std::sort
            (
                devices.begin()
              , devices.end()
              , [get_type_priority] (const auto& lhs, const auto& rhs)
                {
                    // is call getProperties() expensive?
                    return get_type_priority(lhs.getProperties().deviceType) < get_type_priority(rhs.getProperties().deviceType);
                }
            );
        }
    }

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create_physical_device
    (
        const options::Device& options
      , const vk::Instance& instance
      , const std::optional<vk::SurfaceKHR>& surface
    )
    {
        if (options.graphics.enabled && !surface.has_value())
        {
            GE_THROW(invalid_options, "If graphics option enabled, you must pass surface parameter");
        }

        auto devices = instance.enumeratePhysicalDevices();

        sort_by_type(devices);

        const auto& required_layers = get_required_layers(options.validation_layers);

        for (const auto& device : devices)
        {
            try
            {
                all_required_are_available(required_layers, get_available_device_layers(device));

                // TODO: get device feature and match with required features
                [[ maybe_unused ]] const vk::PhysicalDeviceFeatures features = device.getFeatures();

                // TODO: save limits and use them for checking everywhere
                [[ maybe_unused ]] const vk::PhysicalDeviceProperties properties = device.getProperties();

                // TODO: log available memory types and heap sizes
                // TODO: save memory properties and use it everywhere
                [[ maybe_unused ]] const vk::PhysicalDeviceMemoryProperties memory = device.getMemoryProperties();
                [[ maybe_unused ]] const std::vector<vk::ExtensionProperties> extensions = device.enumerateDeviceExtensionProperties();
                [[ maybe_unused ]] const std::vector<vk::LayerProperties> layers = device.enumerateDeviceLayerProperties();

                // from vertex_attribute_descriptions
                // TODO: print flags
                [[ maybe_unused ]] const vk::FormatProperties format_1 = device.getFormatProperties(vk::Format::eR32G32Sfloat);
                assert(format_1.bufferFeatures & vk::FormatFeatureFlagBits::eVertexBuffer);
                [[ maybe_unused ]] const vk::FormatProperties format_2 = device.getFormatProperties(vk::Format::eR32G32B32Sfloat);
                assert(format_2.bufferFeatures & vk::FormatFeatureFlagBits::eVertexBuffer);

                QueueFamilyIndices indices;

                if (options.graphics.enabled)
                {
                    indices.graphics = get_queue_family_index(device, vk::QueueFlagBits::eGraphics);
                    indices.present = get_presentation_queue_family_index(device, surface.value());
                }

                if (options.compute.enabled)
                {
                    indices.compute = get_queue_family_index(device, vk::QueueFlagBits::eCompute);
                }

                if (options.transfer.enabled)
                {
                    indices.transfer = get_queue_family_index(device, vk::QueueFlagBits::eTransfer);
                }

                return {device, indices};
            }
            catch (const device_capabilities_error&)
            {
                continue;
            }
        }

        GE_THROW(device_capabilities_error, "There is no available physical devices");
    }
}
