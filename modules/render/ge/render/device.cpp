#include "device.h"
#include "log_vulkan.hpp"
#include "ge/render/exception.h"

#include <cinttypes>

namespace ge
{
    namespace
    {
        void log_device_details(const vk::PhysicalDevice& device, const Logger& logger) noexcept
        {
            const vk::PhysicalDeviceProperties& properties = device.getProperties();
            logger.log
            (
                LogType::SystemInfo,
                "Device properties:\n", properties
            );
        }

        std::vector<vk::PhysicalDevice> get_physical_devices
        (
            const vk::Instance& instance,
            const Logger& logger
        )
        {
            uint32_t devices_count = 0;
            const vk::Result device_count_enumeration_result = instance.enumeratePhysicalDevices
            (
                &devices_count,
                nullptr
            );
            switch (device_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInitializationFailed:
                GE_THROW_EXPECTED_RESULT(device_count_enumeration_result, "Counting physical devices failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(device_count_enumeration_result, "Counting physical devices failed");
            }
            }

            if (devices_count == 0)
            {
                GE_THROW_EXPECTED_ERROR("There is no physical devices at all");
            }

            std::vector<vk::PhysicalDevice>  devices;
            try
            {
                devices.resize(devices_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for physical devices failed");
            }

            const vk::Result device_enumeration_result = instance.enumeratePhysicalDevices
            (
                &devices_count,
                devices.data()
            );
            switch (device_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInitializationFailed:
                GE_THROW_EXPECTED_RESULT(device_enumeration_result, "Enumerating physical devices failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(device_enumeration_result, "Enumerating physical devices failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                for (const vk::PhysicalDevice& device : devices)
                {
                    log_device_details(device, logger);
                }
            }

            return devices;
        }
    }

    void DeviceData::create_default
    (
        const factory::options::ValidationLayers&,
        const InstanceData& instance,
        const Logger& logger
    )
    {
        get_physical_devices(*instance.instance, logger);
    }
}
