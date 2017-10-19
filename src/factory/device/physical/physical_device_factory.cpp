#include "physical_device_factory.h"
#include "device_requirements.h"
#include "factory_tools.hpp"
#include "device_tools.h"
#include "exception.h"

namespace ge::impl::factory::device::physical
{

    namespace
    {
        std::vector<std::string> get_available_instance_extensions(const vk::PhysicalDevice& device)
        {
            return ge::impl::factory::impl::extensions_names(device.enumerateDeviceExtensionProperties());
        }

        bool has_presentation_support(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
        {
            return get_suitable_queue_family_index(device, surface) >= 0;
        }
    } // unnamed namespace

    bool is_suitable(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface)
    {
        using namespace ge::impl::factory::impl;

        all_required_are_available
        (
            get_required_device_extensions()
          , get_available_instance_extensions(device)
        );

        return has_presentation_support(device, surface);
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
