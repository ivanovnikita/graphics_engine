#include "physical_device_factory.h"
#include "device_requirements.h"
#include "factory_tools.hpp"
#include "exception.h"

namespace ge::impl::factory::device::physical
{

    namespace
    {
        std::vector<std::string> get_available_device_extensions(const vk::PhysicalDevice& device)
        {
            return ge::impl::factory::impl::extensions_names(device.enumerateDeviceExtensionProperties());
        }

        uint32_t get_queue_family_index(const vk::PhysicalDevice& device, vk::QueueFlagBits required_bit)
        {
            const auto queue_family_properties = device.getQueueFamilyProperties();

            if (required_bit == vk::QueueFlagBits::eCompute)
            {
                for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
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
                for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
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

            for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
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
            using namespace ge::impl::factory::impl;

            all_required_are_available
            (
                get_required_device_extensions()
              , get_available_device_extensions(device)
            );

            const auto queue_family_properties = device.getQueueFamilyProperties();

            for (uint32_t i = 0; i < static_cast<uint32_t>(queue_family_properties.size()); ++i)
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
                default: return 3;
                }
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

    } // unnamed namespace

    std::pair<vk::PhysicalDevice, QueueFamilyIndices> create(const vk::Instance& instance, const vk::SurfaceKHR& surface)
    {
        using namespace ge::impl::factory::impl;

        auto devices = instance.enumeratePhysicalDevices();

        sort_by_type(devices);

        for (const auto& device : devices)
        {
            try
            {
                QueueFamilyIndices indices
                {
                    get_queue_family_index(device, vk::QueueFlagBits::eGraphics)
                  , get_presentation_queue_family_index(device, surface)
                  , get_queue_family_index(device, vk::QueueFlagBits::eCompute)
                  , get_queue_family_index(device, vk::QueueFlagBits::eTransfer)
                };

                return {device, indices};
            }
            catch (const device_capabilities_error&)
            {
                continue;
            }
        }

        GE_THROW(device_capabilities_error, "There is no available physical devices");
    }

} // namespace ge::impl::factory::device::physical
