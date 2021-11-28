#include "instance.h"
#include "exception.h"
#include "debug_callback.h"

#include <array>
#include <limits>
#include <string_view>
#include <span>
#include <iostream>

#include <cassert>

namespace ge
{
    namespace
    {
        constexpr size_t MAX_REQUIRED_EXTENSIONS = 3;
        using RequiredExtenstions = std::array<const char*, MAX_REQUIRED_EXTENSIONS>;

        constexpr size_t MAX_REQUIRED_LAYERS = 1;
        using RequiredLayers = std::array<const char*, MAX_REQUIRED_LAYERS>;

        std::span<const char*> get_required_extensions
        (
            RequiredExtenstions& output,
            const factory::options::Instance& options
        ) noexcept
        {
            size_t extensions_count = 0;

            const auto add_extension = [&extensions_count, &output] (const char* extension_name) noexcept
            {
                assert(extensions_count < output.size());
                output[extensions_count] = extension_name;
                ++extensions_count;
            };

            if (options.debug.debug_callback.enabled)
            {
                add_extension(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            }

            if (options.window.enabled)
            {
                add_extension(VK_KHR_SURFACE_EXTENSION_NAME);

                switch (options.window.type)
                {
                case factory::options::WindowType::XCB:
                {
                    add_extension(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
                    break;
                }
                case factory::options::WindowType::WIN32:
                {
#ifdef _WIN32
#if defined(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
                    add_extension(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
    #error Surface extension for WIN32 is absent
#endif
#endif
                     break;
                }
                }
            }

            assert(extensions_count <= output.size());
            return {output.data(), extensions_count};
        }

        std::span<const char*> get_required_layers
        (
            RequiredLayers& output,
            const factory::options::Instance& options
        ) noexcept
        {
            size_t layers_count = 0;

            const auto add_layer = [&layers_count, &output] (const char* layer_name) noexcept
            {
                assert(layers_count < output.size());
                output[layers_count] = layer_name;
                ++layers_count;
            };

            if (options.debug.validation_layers.enabled)
            {
                add_layer("VK_LAYER_KHRONOS_validation");
            }

            assert(layers_count <= output.size());
            return {output.data(), layers_count};
        }

        void check_required_extensions(const std::span<const char*> required_extensions)
        {
            constexpr char* layer_name = nullptr;
            uint32_t property_count = 0;
            const vk::Result prop_count_enumeration_result = vk::enumerateInstanceExtensionProperties
            (
                layer_name,
                &property_count,
                nullptr
            );
            switch (prop_count_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorLayerNotPresent:
                GE_THROW_EXPECTED_RESULT(prop_count_enumeration_result);
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(prop_count_enumeration_result);
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

            const auto process_absent_extensions =
            [
                &absent_extensions_count,
                &absent_extensions_storage
            ]
            {
                if (absent_extensions_count == 0)
                {
                    return;
                }

// TODO: use some error reporter
#ifndef NDEBUG
                std::cerr << "Absent extensions:\n";
                for (size_t i = 0; i < absent_extensions_count; ++i)
                {
                    std::cerr << absent_extensions_storage[i] << "\n";
                }
                std::cerr << std::endl;
#endif
                GE_THROW_EXPECTED_ERROR("Some required extensions are absent");
            };

            if (property_count == 0)
            {
                if (required_extensions.empty())
                {
                    return;
                }

                std::for_each(required_extensions.begin(), required_extensions.end(), add_absent_extension);
                process_absent_extensions();
                return;
            }

            std::vector<vk::ExtensionProperties> properties;
            try
            {
                properties.resize(property_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation error");
            }

            const vk::Result prop_enumeration_result = vk::enumerateInstanceExtensionProperties
            (
                layer_name,
                &property_count,
                properties.data()
            );
            switch (prop_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
            case vk::Result::eErrorLayerNotPresent:
                GE_THROW_EXPECTED_RESULT(prop_enumeration_result);
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(prop_enumeration_result);
            }
            }

            // TODO: log available properties

            for (const char* required_extension : required_extensions)
            {
                bool found = false;
                const std::string_view required{required_extension};
                for (const vk::ExtensionProperties& property : properties)
                {
                    if (std::string_view{property.extensionName.data()} == required)
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

        void check_required_layers(const std::span<const char*> required_layers)
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
                GE_THROW_EXPECTED_RESULT(layer_count_enumeration_result);
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_count_enumeration_result);
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

            const auto process_absent_layers =
            [
                &absent_layers_count,
                &absent_layers_storage
            ]
            {
                if (absent_layers_count == 0)
                {
                    return;
                }

// TODO: use some error reporter
#ifndef NDEBUG
                std::cerr << "Absent layers:\n";
                for (size_t i = 0; i < absent_layers_count; ++i)
                {
                    std::cerr << absent_layers_storage[i] << "\n";
                }
                std::cerr << std::endl;
#endif
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

            std::vector<vk::LayerProperties> properties;
            try
            {
                properties.resize(layer_count);
            }
            catch (const std::bad_alloc& e)
            {
                GE_THROW_EXPECTED_ERROR("Allocation error");
            }

            const vk::Result layer_enumeration_result = vk::enumerateInstanceLayerProperties
            (
                &layer_count,
                properties.data()
            );
            switch (layer_enumeration_result)
            {
            case vk::Result::eSuccess:
            case vk::Result::eIncomplete:
                 break;
            case vk::Result::eErrorOutOfHostMemory:
            case vk::Result::eErrorOutOfDeviceMemory:
                GE_THROW_EXPECTED_RESULT(layer_enumeration_result);
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(layer_enumeration_result);
            }
            }

            // TODO: log available properties

            for (const char* required_layer : required_layers)
            {
                bool found = false;
                const std::string_view required{required_layer};
                for (const vk::LayerProperties& property : properties)
                {
                    if (std::string_view{property.layerName.data()} == required)
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

        vk::UniqueInstance create_instance
        (
            const std::span<const char*> required_extensions,
            const std::span<const char*> required_layers
        )
        {
            // TODO: check available vulkan version by calling vkEnumerateInstanceVersion (from v1.1)

            const vk::ApplicationInfo application_info
            {
                "graphics engine",
                VK_MAKE_VERSION(1, 0, 0),
                "no engine",
                VK_MAKE_VERSION(1, 0, 0),
                VK_API_VERSION_1_1
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
                GE_THROW_EXPECTED_RESULT(result);
            default:
            {
                GE_THROW_UNEXPECTED_RESULT(result);
            }
            }

            return vk::UniqueInstance{std::move(instance), {vk::Optional<const vk::AllocationCallbacks>(nullptr)}};
        }
    }

    InstanceData InstanceData::create_default(const factory::options::Instance& options)
    {
        RequiredExtenstions required_extensions_storage;
        const std::span<const char*> required_extensions = get_required_extensions
        (
            required_extensions_storage,
            options
        );

        if (not required_extensions.empty())
        {
            check_required_extensions(required_extensions);
        }

        RequiredLayers required_layers_storage;
        const std::span<const char*> required_layers = get_required_layers
        (
            required_layers_storage,
            options
        );

        if (not required_layers.empty())
        {
            check_required_layers(required_layers);
        }

        vk::UniqueInstance instance = create_instance(required_extensions, required_layers);
        vk::UniqueDebugReportCallbackEXT debug_callback = init_default_debug_callback(*instance);
        return InstanceData
        {
            std::move(instance),
            std::move(debug_callback)
        };
    }
}
