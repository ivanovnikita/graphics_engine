#include "ge/common/safe_cast.hpp"
#include "ge/render/factory/instance.h"
#include "ge/render/factory/tools.hpp"

//#include "ge/experimental/allocator.h"
//#include "ge/experimental/vector.hpp"
//#include "ge/experimental/result.hpp"

#include <vector>

namespace ge::factory
{
    namespace
    {
//        template <typename T>
//        Result
//        <
//            Ref<Vector<T>>,
//            Errors
//            <
//                Allocator::AllocationError
//            >
//        > resize(Vector<T>& vec, Allocator& allocator, const size_t count) noexcept
//        {
//            return vec.resize(allocator, count)
//                .then
//                (
//                    [&vec] (Ok) noexcept
//                    {
//                        return std::ref(vec);
//                    }
//                );
//        }

//        template <typename T>
//        Result
//        <
//            Ref<T>,
//            Errors
//            <
//                Allocator::AllocationError
//            >
//        > resize_and_get_view(Vector<T>& vec, Allocator& allocator, const size_t count)
//        {
//            assert(count > 0);
//            return resize(vec, allocator, count)
//                .then
//                (
//                    [] (Vector<T>& vec) noexcept
//                    {
//                        return vec[0]
//                            .on_error
//                            (
//                                [] (auto&&) noexcept -> Errors<Allocator::AllocationError>
//                                {
//                                    assert(false);
//                                    __builtin_unreachable();
//                                }
//                            );
//                    }
//                );
//        }

//        Result
//        <
//            Vector<vk::ExtensionProperties>,
//            Errors
//            <
//                Allocator::AllocationError,
//                VkErrorOutOfHostMemory,
//                VkErrorOutOfDeviceMemory,
//                VkErrorLayerNotPresent,
//                VkUnexpectedError
//            >
//        > enumerate_instance_extension_properties(Allocator& allocator)
//        {
//            Vector<vk::ExtensionProperties> properties;
//            uint32_t property_count = 0;

//            vk::Result vk_result;
//            constexpr const char* layer_name = nullptr;

//            do
//            {
//                vk_result = vk::enumerateInstanceExtensionProperties(layer_name, &property_count, nullptr);
//                if ((vk_result == vk::Result::eSuccess) and property_count > 0)
//                {
//                    using AllocRes = Option<Errors<Allocator::AllocationError>>;

//                    AllocRes alloc_result = resize_and_get_view(properties, allocator, property_count)
//                        .match
//                        (
//                            [&] (vk::ExtensionProperties& front) noexcept -> AllocRes
//                            {
//                                vk_result = vk::enumerateInstanceExtensionProperties
//                                (
//                                    layer_name,
//                                    &property_count,
//                                    &front
//                                );

//                                if (vk_result == vk::Result::eSuccess)
//                                {
//                                    assert(property_count <= properties.size());
//                                    [[ maybe_unused ]] auto alloc_result = properties.resize(allocator, property_count);
//                                    assert(alloc_result.is_ok());
//                                }

//                                return none;
//                            },
//                            [] (auto&& e) noexcept -> AllocRes
//                            {
//                                return std::move(e);
//                            }
//                        );

//                    if (result.is_error())
//                    {
//                        return result;
//                    }
//                }
//            } while (vk_result == vk::Result::eIncomplete); // TODO: limit iterations

//            switch (vk_result)
//            {
//            case vk::Result::eSuccess: return std::move(properties);
//            case vk::Result::eErrorOutOfHostMemory: return VkErrorOutOfHostMemory{};
//            case vk::Result::eErrorOutOfDeviceMemory: return VkErrorOutOfDeviceMemory{};
//            default: return VkUnexpectedError{vk_result};
//            }
//        }

        std::vector<std::string> get_available_instance_extensions()
        {
            return extensions_names(vk::enumerateInstanceExtensionProperties());
        }

        std::vector<std::string> get_available_instance_layers()
        {
            return layers_names(vk::enumerateInstanceLayerProperties());
        }

        std::vector<const char*> get_required_extensions
        (
            const options::DebugCallback& option_debug_callback
          , const options::Window& option_window
        )
        {
            std::vector<const char*> extensions;

            if (option_debug_callback.enabled)
            {
                extensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
            }

            if (option_window.enabled)
            {
                extensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);

                switch (option_window.type)
                {
                case options::WindowType::XCB:
                {
                    extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
                } break;
                case options::WindowType::WIN32:
                {
#ifdef _WIN32
#if defined(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
                    extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#else
    #error Surface extension for WIN32 is absent
#endif
#endif
                } break;
                }
            }

            return extensions;
        }
    }

    vk::UniqueInstance create_instance(const options::Instance& options)
    {
        // TODO: check available vulkan version by calling vkEnumerateInstanceVersion (from v1.1)

        const vk::ApplicationInfo application_info
        {
            "graphics engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , "no engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , VK_API_VERSION_1_1
        };

        const auto required_extensions = get_required_extensions(options.debug.debug_callback, options.window);
        const auto required_layers = get_required_layers(options.debug.validation_layers);

        all_required_are_available(required_extensions, get_available_instance_extensions());
        all_required_are_available(required_layers, get_available_instance_layers());

        const vk::InstanceCreateInfo create_info
        {
            vk::InstanceCreateFlags()
          , &application_info
          , safe_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , safe_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
        };

        return vk::createInstanceUnique(create_info);
    }
}
