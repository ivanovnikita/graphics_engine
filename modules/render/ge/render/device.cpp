#include "device.h"
#include "log_vulkan.hpp"
#include "invoke_for_downcasted.h"
#include "ge/render/exception.h"

#include <cinttypes>

namespace ge
{
    namespace
    {
        template <typename F>
        void for_each_in_chain(const vk::BaseInStructure& in, F&& func)
        {
            const vk::BaseInStructure* current = &in;
            while (current != nullptr)
            {
                invoke_for_downcasted(*current, func);
                current = current->pNext;
            }
        }

        void log_device_properties2(const vk::PhysicalDevice& device, const Logger& logger) noexcept
        {
            const vk::PhysicalDeviceProperties2 properties = device.getProperties2();
            if (properties.pNext != nullptr)
            {
                for_each_in_chain
                (
                    *reinterpret_cast<vk::BaseInStructure*>(properties.pNext),
                    [&logger] (const auto& structure)
                    {
                        logger.log(LogType::SystemInfo, structure);
                    }
                );
            }
        }

        void log_device_details
        (
            const InstanceData& instance,
            const vk::PhysicalDevice& device,
            const Logger& logger
        ) noexcept
        {
            const vk::PhysicalDeviceProperties& properties = device.getProperties();

            const Version device_api_version = Version::from_vulkan_version(properties.apiVersion);

            logger.log
            (
                LogType::SystemInfo,
                "Device properties:\n",
                "apiVersion: ", device_api_version, "\n",
                properties
            );

            if
            (
                instance.api_version.major == 1 and
                instance.api_version.minor >= 1 and
                device_api_version.major == 1 and
                device_api_version.minor >= 1
            )
            {
                log_device_properties2(device, logger);
            }
        }

        std::vector<vk::PhysicalDevice> get_physical_devices
        (
            const InstanceData& instance,
            const Logger& logger
        )
        {
            uint32_t devices_count = 0;
            const vk::Result device_count_enumeration_result = instance.instance->enumeratePhysicalDevices
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

            const vk::Result device_enumeration_result = instance.instance->enumeratePhysicalDevices
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
                    log_device_details(instance, device, logger);
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
        get_physical_devices(instance, logger);
    }
}
