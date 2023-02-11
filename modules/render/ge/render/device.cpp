#include "device.h"
#include "log_vulkan.hpp"
//#include "invoke_for_downcasted.h"
#include "ge/render/exception.h"

#include <limits>

#include <cinttypes>

namespace ge
{
    namespace
    {
        constexpr size_t MAX_REQUIRED_EXTENSIONS = 1;
        using RequiredExtenstions = std::array<const char*, MAX_REQUIRED_EXTENSIONS>;

        constexpr size_t MAX_REQUIRED_LAYERS = 1;
        using RequiredLayers = std::array<const char*, MAX_REQUIRED_LAYERS>;

//        template <typename F>
//        void for_each_in_chain(const vk::BaseInStructure& in, F&& func)
//        {
//            const vk::BaseInStructure* current = &in;
//            while (current != nullptr)
//            {
//                invoke_for_downcasted(*current, func);
//                current = current->pNext;
//            }
//        }

        // RIP compilation time and binary size
//        void log_device_properties2(const vk::PhysicalDeviceProperties2& properties, const Logger& logger) noexcept
//        {
//            if (properties.pNext != nullptr)
//            {
//                for_each_in_chain
//                (
//                    *reinterpret_cast<vk::BaseInStructure*>(properties.pNext),
//                    [&logger] (const auto& structure)
//                    {
//                        logger.log(LogType::SystemInfo, structure);
//                    }
//                );
//            }
//        }

        // RIP compilation time and binary size
//        void log_device_features2(const vk::PhysicalDeviceFeatures2& features, const Logger& logger) noexcept
//        {
//            if (features.pNext != nullptr)
//            {
//                for_each_in_chain
//                (
//                    *reinterpret_cast<vk::BaseInStructure*>(features.pNext),
//                    [&logger] (const auto& structure)
//                    {
//                        logger.log(LogType::SystemInfo, structure);
//                    }
//                );
//            }
//        }

        void log_device_details
        (
            const PhysicalDeviceData& device,
            const Logger& logger
        ) noexcept
        {
            const Version device_api_version = Version::from_vulkan_version(device.properties.apiVersion);

            logger.log
            (
                LogType::SystemInfo,
                "Device properties:\n",
                "apiVersion: ", device_api_version, "\n",
                device.properties
            );

            if (device.properties2.has_value())
            {
                // RIP compilation time and binary size
                //log_device_properties2(*device.properties2, logger);
            }

            logger.log(LogType::SystemInfo, "Device features:\n", device.features);

            if (device.features2.has_value())
            {
                // RIP compilation time and binary size
                //log_device_features2(*device.features2, logger);
            }

            logger.log(LogType::SystemInfo, "Device memory:\n", device.memory);
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
                case vk::PhysicalDeviceType::eVirtualGpu: return 3;
                case vk::PhysicalDeviceType::eOther: return 4;
                }

                __builtin_unreachable();
            };
            std::sort
            (
                devices.begin(),
                devices.end(),
                [&get_type_priority] (const auto& lhs, const auto& rhs)
                {
                    // is call getProperties() expensive?
                    return get_type_priority(lhs.getProperties().deviceType) < get_type_priority(rhs.getProperties().deviceType);
                }
            );
        }

        std::vector<vk::PhysicalDevice> get_physical_devices(const InstanceData& instance)
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

            std::vector<vk::PhysicalDevice> devices;
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

            return devices;
        }

        std::vector<vk::QueueFamilyProperties> get_queue_family_properties(const vk::PhysicalDevice& device)
        {
            uint32_t queue_family_count = 0;
            device.getQueueFamilyProperties(&queue_family_count, nullptr);

            std::vector<vk::QueueFamilyProperties> result;
            try
            {
                result.resize(queue_family_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for physical devices failed");
            }

            device.getQueueFamilyProperties(&queue_family_count, result.data());

            return result;
        }

        std::optional<size_t> select_queue_family_index
        (
            const std::span<vk::QueueFamilyProperties>& queues,
            const vk::QueueFlagBits required_bit
        )
        {
            // TODO: do we really need to select queues in a such way?
            if (required_bit == vk::QueueFlagBits::eCompute)
            {
                for (size_t i = 0; i < queues.size(); ++i)
                {
                    if
                    (
                        queues[i].queueFlags & required_bit and
                        queues[i].queueCount > 0 and
                        not (queues[i].queueFlags & vk::QueueFlagBits::eGraphics)
                    )
                    {
                        return i;
                    }
                }
            }

            if (required_bit == vk::QueueFlagBits::eTransfer)
            {
                for (size_t i = 0; i < queues.size(); ++i)
                {
                    if
                    (
                        queues[i].queueFlags & required_bit and
                        queues[i].queueCount > 0 and
                        not (queues[i].queueFlags & vk::QueueFlagBits::eGraphics) and
                        not (queues[i].queueFlags & vk::QueueFlagBits::eCompute)
                    )
                    {
                        return i;
                    }
                }
            }

            for (size_t i = 0; i < queues.size(); ++i)
            {
                if
                (
                    queues[i].queueFlags & required_bit and
                    queues[i].queueCount > 0
                )
                {
                    return i;
                }
            }

            return std::nullopt;
        }

        std::optional<uint32_t> select_presentation_queue_family_index
        (
            const vk::PhysicalDevice& device,
            const std::span<vk::QueueFamilyProperties>& queues,
            const vk::SurfaceKHR& surface
        )
        {
            assert(queues.size() <= std::numeric_limits<uint32_t>::max());
            for (size_t i = 0; i < queues.size(); ++i)
            {
                if
                (
                    device.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface) == VK_TRUE and
                    queues[i].queueCount > 0
                )
                {
                    return i;
                }
            }

            return std::nullopt;
        }

        PhysicalDeviceData get_physical_device_data
        (
            const InstanceData& instance,
            const vk::PhysicalDevice& device,
            std::optional<std::reference_wrapper<const vk::SurfaceKHR>> surface
        )
        {
            PhysicalDeviceData result
            {
                .physical_device = device,
                .properties = device.getProperties(),
                .properties2 = std::nullopt,
                .features = device.getFeatures(),
                .features2 = std::nullopt,
                .memory = device.getMemoryProperties(),
                .queue_properties = get_queue_family_properties(device),
                .graphics_queue_index = std::nullopt,
                .present_queue_index = std::nullopt,
                .compute_queue_index = std::nullopt,
                .transfer_queue_index = std::nullopt
            };
            result.graphics_queue_index = select_queue_family_index(result.queue_properties, vk::QueueFlagBits::eGraphics);
            result.compute_queue_index = select_queue_family_index(result.queue_properties, vk::QueueFlagBits::eCompute);
            result.transfer_queue_index = select_queue_family_index(result.queue_properties, vk::QueueFlagBits::eTransfer);

            if (surface.has_value())
            {
                // TODO: check that VK_KHR_surface is enabled in Instance
                result.present_queue_index = select_presentation_queue_family_index
                (
                    device,
                    result.queue_properties,
                    *surface
                );
            }

            const Version device_api_version = Version::from_vulkan_version(result.properties.apiVersion);

            if
            (
                instance.api_version.major == 1 and
                instance.api_version.minor >= 1 and
                device_api_version.major == 1 and
                device_api_version.minor >= 1
            )
            {
                result.properties2 = device.getProperties2();
            }

            if
            (
                instance.api_version.major == 1 and
                instance.api_version.minor >= 1 and
                device_api_version.major == 1 and
                device_api_version.minor >= 1
            )
            {
                result.features2 = device.getFeatures2();
            }

            return result;
        }

        bool is_all_required_extensions_available
        (
            const vk::PhysicalDevice& device,
            const std::span<const char*> required_extensions,
            const Logger& logger
        )
        {
            constexpr char* layer_name = nullptr;
            uint32_t ext_count = 0;
            const vk::Result ext_count_enumeration_result = device.enumerateDeviceExtensionProperties
            (
                layer_name,
                &ext_count,
                nullptr
            );
            switch (ext_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorLayerNotPresent:
                GE_THROW_EXPECTED_RESULT(ext_count_enumeration_result, "Counting physical device extensions failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(ext_count_enumeration_result, "Counting physical device extensions failed");
            }
            }

            RequiredExtenstions absent_extensions_storage;
            assert(absent_extensions_storage.size() >= required_extensions.size());

            size_t absent_extensions_count = 0;
            const auto add_absent_extension =
            [
                &absent_extensions_count,
                &absent_extensions_storage
            ] (const char* extension_name) noexcept
            {
                assert(absent_extensions_count < absent_extensions_storage.size());
                absent_extensions_storage[absent_extensions_count] = extension_name;
                ++absent_extensions_count;
            };

            const auto log_absent_extensions =
            [
                &absent_extensions_count,
                &absent_extensions_storage,
                &logger
            ]
            {
                if (absent_extensions_count == 0)
                {
                    return;
                }

                if (logger.enabled(LogType::SystemInfo))
                {
                    logger.log(LogType::SystemInfo, "Absent extensions:\n");
                    for (size_t i = 0; i < absent_extensions_count; ++i)
                    {
                        logger.log(LogType::SystemInfo, "- ", absent_extensions_storage[i], "\n");
                    }
                }
            };

            if (ext_count == 0)
            {
                if (required_extensions.empty())
                {
                    return true;
                }

                std::for_each(required_extensions.begin(), required_extensions.end(), add_absent_extension);
                log_absent_extensions();
                return false;
            }

            std::vector<vk::ExtensionProperties> extensions;
            try
            {
                extensions.resize(ext_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for physical device extension properties failed");
            }

            const vk::Result ext_enumeration_result = device.enumerateDeviceExtensionProperties
            (
                layer_name,
                &ext_count,
                extensions.data()
            );
            switch (ext_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorLayerNotPresent:
                GE_THROW_EXPECTED_RESULT(ext_enumeration_result, "Enumeration physical device extensions failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(ext_enumeration_result, "Enumeration physical device extensions failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                logger.log(LogType::SystemInfo, "Available physical device extensions:\n", extensions);
            }

            for (const char* required_extension : required_extensions)
            {
                bool found = false;
                const std::string_view required{required_extension};
                for (const vk::ExtensionProperties& extention : extensions)
                {
                    if (std::string_view{extention.extensionName.data()} == required)
                    {
                        found = true;
                        break;
                    }
                }
                if (not found)
                {
                    add_absent_extension(required_extension);
                }
            }

            log_absent_extensions();

            return absent_extensions_count == 0;
        }

        bool is_all_required_layers_available
        (
            const vk::PhysicalDevice& device,
            const std::span<const char*> required_layers,
            const Logger& logger
        )
        {
            uint32_t layer_count = 0;
            const vk::Result layer_count_enumeration_result = device.enumerateDeviceLayerProperties
            (
                &layer_count,
                nullptr
            );
            switch (layer_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(layer_count_enumeration_result, "Counting physical device layers failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_count_enumeration_result, "Counting physical device layers failed");
            }
            }

            RequiredExtenstions absent_layers_storage;
            assert(absent_layers_storage.size() >= required_layers.size());

            size_t absent_layers_count = 0;
            const auto add_absent_layer =
            [
                &absent_layers_count,
                &absent_layers_storage
            ] (const char* layer_name) noexcept
            {
                assert(absent_layers_count < absent_layers_storage.size());
                absent_layers_storage[absent_layers_count] = layer_name;
                ++absent_layers_count;
            };

            const auto log_absent_layers =
            [
                &absent_layers_count,
                &absent_layers_storage,
                &logger
            ]
            {
                if (absent_layers_count == 0)
                {
                    return;
                }

                if (logger.enabled(LogType::SystemInfo))
                {
                    logger.log(LogType::SystemInfo, "Absent layers:\n");
                    for (size_t i = 0; i < absent_layers_count; ++i)
                    {
                        logger.log(LogType::SystemInfo, "- ", absent_layers_storage[i], "\n");
                    }
                }
            };

            if (layer_count == 0)
            {
                if (required_layers.empty())
                {
                    return true;
                }

                std::for_each(required_layers.begin(), required_layers.end(), add_absent_layer);
                log_absent_layers();
                return false;
            }

            std::vector<vk::LayerProperties> layers;
            try
            {
                layers.resize(layer_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for physical device layer properties failed");
            }

            const vk::Result layer_enumeration_result = device.enumerateDeviceLayerProperties
            (
                &layer_count,
                layers.data()
            );
            switch (layer_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(layer_enumeration_result, "Enumeration physical device layers failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_enumeration_result, "Enumeration physical device layers failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                logger.log(LogType::SystemInfo, "Available physical device layers:\n", layers);
            }

            for (const char* required_layer : required_layers)
            {
                bool found = false;
                const std::string_view required{required_layer};
                for (const vk::LayerProperties& layer : layers)
                {
                    if (std::string_view{layer.layerName.data()} == required)
                    {
                        found = true;
                        break;
                    }
                }
                if (not found)
                {
                    add_absent_layer(required_layer);
                }
            }

            log_absent_layers();

            return absent_layers_count == 0;
        }
    }

    void DeviceData::create_default
    (
        const factory::options::ValidationLayers&,
        const InstanceData& instance,
        std::optional<std::reference_wrapper<const vk::SurfaceKHR>> surface,
        const Logger& logger
    )
    {
        std::vector<vk::PhysicalDevice> devices = get_physical_devices(instance);
        sort_by_type(devices);

        std::optional<PhysicalDeviceData> best_fit_device;
        for (const vk::PhysicalDevice& device : devices)
        {
            PhysicalDeviceData device_data = get_physical_device_data(instance, device, surface);

            if (logger.enabled(LogType::SystemInfo))
            {
                log_device_details(device_data, logger);
            }

            const bool all_required_ext_available = is_all_required_extensions_available
            (
                device,
                {},
                logger
            );

            const bool all_required_layers_available = is_all_required_layers_available
            (
                device,
                {},
                logger
            );

            if
            (
                not best_fit_device.has_value() and
                all_required_ext_available and
                all_required_layers_available and
                // TODO
                device_data.graphics_queue_index.has_value() and
                device_data.present_queue_index.has_value()
            )
            {
                best_fit_device.emplace(std::move(device_data));
            }
        }

        if (not best_fit_device.has_value())
        {
            GE_THROW_EXPECTED_ERROR("There is no available physical devices");
        }
    }
}
