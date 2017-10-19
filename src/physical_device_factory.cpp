#include "physical_device_factory.h"
#include "physical_device_requirements.h"
#include "factory_tools.hpp"
#include "exception.h"

#include <optional>

namespace ge::impl::factory::device::physical
{

    namespace impl
    {
        std::vector<std::string> get_available_instance_extensions(const vk::PhysicalDevice& device)
        {
            return ge::impl::factory::impl::extensions_names(device.enumerateDeviceExtensionProperties());
        }

        bool has_presentation_support(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
        {
            const auto queue_family_properties = device.getQueueFamilyProperties();
            const auto required_queue_family_flags = get_required_queue_family_flags();

            for (uint32_t i = 0; i < queue_family_properties.size(); ++i)
            {
                if
                (
                    queue_family_properties[i].queueCount > 0
                 && queue_family_properties[i].queueFlags & required_queue_family_flags
                 && device.getSurfaceSupportKHR(i, surface)
                )
                {
                    return true;
                }
            }
            return false;
        }
    } // namespace impl

    bool is_suitable(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        using namespace ge::impl::factory::impl;

        all_required_are_available
        (
            get_required_physical_device_extensions()
          , impl::get_available_instance_extensions(device)
        );

        return impl::has_presentation_support(device, surface);
    }

    vk::PhysicalDevice create(const vk::Instance& instance, const vk::SurfaceKHR& surface)
    {
        const auto devices = instance.enumeratePhysicalDevices();

        if
        (
            auto device = std::find_if
            (
                devices.begin()
              , devices.end()
              , [&surface] (const auto& device)
              {
                  return is_suitable(device, surface);
              }
            );
            device != devices.end()
        )
        {
            return *device;
        }

        GE_THROW(device_capabilities_error, "There is no available physical devices");
    }

} // namespace ge::impl::factory::device::physical
