#include "instance.h"
#include "exception.h"
#include "debug_callback.h"
#include "log_vulkan.hpp"

#include <array>
#include <limits>
#include <string_view>
#include <span>

#include <cassert>

namespace ge
{
    namespace
    {
        void check_required_extensions(const std::span<const char*> required_extensions, const Logger& logger)
        {
            constexpr char* layer_name = nullptr;
            uint32_t ext_count = 0;
            const vk::Result ext_count_enumeration_result = vk::enumerateInstanceExtensionProperties
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
                GE_THROW_EXPECTED_RESULT(ext_count_enumeration_result, "Counting instance extensions failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(ext_count_enumeration_result, "Counting instance extensions failed");
            }
            }

            InstanceExtensionStorage absent_extensions_storage;
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

            const auto process_absent_extensions =
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

                if (logger.enabled(LogType::ErrorDetails))
                {
                    logger.log(LogType::ErrorDetails, "Absent extensions:\n");
                    for (size_t i = 0; i < absent_extensions_count; ++i)
                    {
                        logger.log(LogType::ErrorDetails, "- ", absent_extensions_storage[i], "\n");
                    }
                }
                GE_THROW_EXPECTED_ERROR("Some required instance extensions are absent");
            };

            if (ext_count == 0)
            {
                if (required_extensions.empty())
                {
                    return;
                }

                std::for_each(required_extensions.begin(), required_extensions.end(), add_absent_extension);
                process_absent_extensions();
                return;
            }

            std::vector<vk::ExtensionProperties> extensions;
            try
            {
                extensions.resize(ext_count);
            }
            catch (const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for instance extension properties failed");
            }

            const vk::Result ext_enumeration_result = vk::enumerateInstanceExtensionProperties
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
                GE_THROW_EXPECTED_RESULT(ext_enumeration_result, "Enumeration instance extensions failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(ext_enumeration_result, "Enumeration instance extensions failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                logger.log(LogType::SystemInfo, "Available instance extensions:\n", extensions);
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

            process_absent_extensions();
        }

        void check_required_layers(const std::span<const char*> required_layers, const Logger& logger)
        {
            uint32_t layer_count = 0;
            const vk::Result layer_count_enumeration_result = vk::enumerateInstanceLayerProperties
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
                GE_THROW_EXPECTED_RESULT(layer_count_enumeration_result, "Counting instance layers failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_count_enumeration_result, "Counting instance layers failed");
            }
            }

            InstanceLayersStorage absent_layers_storage;
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

            const auto process_absent_layers =
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

                if (logger.enabled(LogType::ErrorDetails))
                {
                    logger.log(LogType::ErrorDetails, "Absent layers:\n");
                    for (size_t i = 0; i < absent_layers_count; ++i)
                    {
                        logger.log(LogType::ErrorDetails, "- ", absent_layers_storage[i], "\n");
                    }
                }
                GE_THROW_EXPECTED_ERROR("Some required layers are absent");
            };

            if (layer_count == 0)
            {
                if (required_layers.empty())
                {
                    return;
                }

                std::for_each(required_layers.begin(), required_layers.end(), add_absent_layer);
                process_absent_layers();
                return;
            }

            std::vector<vk::LayerProperties> layers;
            try
            {
                layers.resize(layer_count);
            }
            catch (const std::bad_alloc&)
            {
                GE_THROW_EXPECTED_ERROR("Allocation for instance layer properties failed");
            }

            const vk::Result layer_enumeration_result = vk::enumerateInstanceLayerProperties
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
                GE_THROW_EXPECTED_RESULT(layer_enumeration_result, "Enumeration instance layers failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_enumeration_result, "Enumeration instance layers failed");
            }
            }

            if (logger.enabled(LogType::SystemInfo))
            {
                logger.log(LogType::SystemInfo, "Available instance layers:\n", layers);
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

            process_absent_layers();
        }

        Version get_instance_api_version(const Logger& logger)
        {
            // vkEnumerateInstanceVersion introduced in api v1.1
            const auto enumeration_version_func = reinterpret_cast<PFN_vkEnumerateInstanceVersion>
            (
                vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion")
            );

            uint32_t version = VK_API_VERSION_1_0;

            if (enumeration_version_func == nullptr)
            {
                logger.log
                (
                    LogType::SystemInfo,
                    "Function ptr 'vkEnumerateInstanceVersion' is null, perhaps api version is 1.0"
                );
            }
            else
            {
                const vk::Result result = vk::enumerateInstanceVersion(&version);
                switch (result)
                {
                case vk::Result::eSuccess: break;
                case vk::Result::eErrorOutOfHostMemory:
                default:
                {
                    if (logger.enabled(LogType::SystemInfo))
                    {
                        logger.log
                        (
                            LogType::SystemInfo,
                            "'vkEnumerateInstanceVersion' returned error ",result,
                            ", perhaps api version is 1.0"
                        );
                    }
                }
                }
            }

            const Version result = Version::from_vulkan_version(version);

            if (logger.enabled(LogType::SystemInfo))
            {
                logger.log
                (
                    LogType::SystemInfo,
                    "Available api version: ", result, "\n"
                );
                if (result.variant != 0)
                {
                    logger.log
                    (
                        LogType::Error,
                        "Api variant = ", result.variant, " is not equal 0, perhaps application requires to be modified to use it"
                    );
                }
            }

            return result;
        }

        std::pair<vk::UniqueInstance, Version> create_instance
        (
            const std::span<const char*> required_extensions,
            const std::span<const char*> required_layers,
            const Logger& logger
        )
        {
            const Version available_api_version = get_instance_api_version(logger);
            const Version version_to_use
            {
                .major = available_api_version.major,
                .minor = available_api_version.minor,
                .patch = 0,
                .variant = 0
            };

            const vk::ApplicationInfo application_info
            {
                "graphics engine",
                VK_MAKE_VERSION(1, 0, 0),
                "no engine",
                VK_MAKE_VERSION(1, 0, 0),
                version_to_use.to_vulkan_version()
            };

            assert(required_extensions.size() < std::numeric_limits<uint32_t>::max());
            assert(required_layers.size() < std::numeric_limits<uint32_t>::max());
            const vk::InstanceCreateInfo create_info
            {
                vk::InstanceCreateFlags()
              , &application_info
              , static_cast<uint32_t>(required_layers.size())
              , required_layers.data()
              , static_cast<uint32_t>(required_extensions.size())
              , required_extensions.data()
            };

            vk::Instance instance;
            const vk::Result result = vk::createInstance(&create_info, nullptr, &instance);
            switch (result)
            {
            case vk::Result::eSuccess:
                break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorInitializationFailed:
            case vk::Result::eErrorLayerNotPresent:
            case vk::Result::eErrorExtensionNotPresent:
            case vk::Result::eErrorIncompatibleDriver:
                GE_THROW_EXPECTED_RESULT(result, "Instance creation failed");
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result, "Instance creation failed");
            }
            }

            return
            {
                vk::UniqueInstance{std::move(instance), {vk::Optional<const vk::AllocationCallbacks>(nullptr)}},
                version_to_use
            };
        }
    }

    InstanceData InstanceData::create_default
    (
        InstanceLayerFlags required_layers,
        InstanceExtensionFlags required_extensions,
        const Logger& logger
    )
    {
        InstanceExtensionStorage required_extensions_storage;
        const std::span<const char*> required_extension_names = get_required_extensions
        (
            required_extensions_storage,
            required_extensions
        );

        if (not required_extension_names.empty())
        {
            check_required_extensions(required_extension_names, logger);
        }

        InstanceLayersStorage required_layers_storage;
        const std::span<const char*> required_layer_names = get_required_layers
        (
            required_layers_storage,
            required_layers
        );

        if (not required_layer_names.empty())
        {
            check_required_layers(required_layer_names, logger);
        }

        auto [instance, api_version] = create_instance
        (
            required_extension_names,
            required_layer_names,
            logger
        );
        vk::UniqueDebugReportCallbackEXT debug_callback = init_default_debug_callback(*instance, logger);
        return InstanceData
        {
            .instance = std::move(instance),
            .api_version = api_version,
            .enabled_layers = std::move(required_layers),
            .enabled_extension = std::move(required_extensions),
            .debug_callback = std::move(debug_callback)
        };
    }
}
