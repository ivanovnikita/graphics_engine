#pragma once

#include "instance_factory.hpp"
#include "factory_tools.hpp"

#include <array>

namespace ge::impl::factory::instance
{

    namespace impl
    {

        std::vector<std::string> get_available_instance_extensions()
        {
            return ge::impl::factory::impl::extensions_names(vk::enumerateInstanceExtensionProperties());
        }

        std::vector<std::string> get_available_instance_layers()
        {
            return ge::impl::factory::impl::layers_names(vk::enumerateInstanceLayerProperties());
        }

        template
        <
            typename OPTION_DEBUG_CALLBACK
          , typename OPTION_WINDOW
        >
        constexpr size_t get_required_extensions_count()
        {
            size_t count = 0;
            if (typename OPTION_DEBUG_CALLBACK::enabled())
            {
                ++count;
            }
            if (typename OPTION_WINDOW::enabled())
            {
                count += 2u;
            }
            return count;
        }

        template
        <
            typename OPTION_DEBUG_CALLBACK
          , typename OPTION_WINDOW
        >
        constexpr auto get_required_extensions()
        {
            constexpr size_t size = get_required_extensions_count<OPTION_DEBUG_CALLBACK, OPTION_WINDOW>();

            std::array<const char*, size> extensions{};

            size_t i = 0;
            if (typename OPTION_DEBUG_CALLBACK::enabled())
            {
                extensions[i] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
                ++i;
            }

            if (typename OPTION_WINDOW::enabled())
            {
                extensions[i] = VK_KHR_SURFACE_EXTENSION_NAME;
                ++i;

                switch (OPTION_WINDOW::type)
                {
                case WindowType::XCB:
                {
                    extensions[i] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
                    ++i;
                } break;
                case WindowType::WIN32:
                {
#if defined(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
                    extensions[i] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
                    ++i;
#endif
                } break;
                default: break;
                }
            }

            return extensions;
        }

        template<typename OPTION_VALIDATION_LAYERS>
        constexpr size_t get_required_layers_count()
        {
            size_t count = 0;
            if (typename OPTION_VALIDATION_LAYERS::enabled())
            {
                ++count;
            }
            return count;
        }

        template<typename OPTION_VALIDATION_LAYERS>
        constexpr auto get_required_layers()
        {
            constexpr size_t size = get_required_layers_count<OPTION_VALIDATION_LAYERS>();

            std::array<const char*, size> layers{};

            size_t i = 0;
            if (typename OPTION_VALIDATION_LAYERS::enabled())
            {
                layers[i] = "VK_LAYER_LUNARG_standard_validation";
                ++i;
            }
            return layers;
        }

    } // namespace impl

    template<typename OPTIONS>
    vk::UniqueInstance create()
    {
        using namespace ge::impl::factory::impl;
        using namespace impl;

        const vk::ApplicationInfo application_info
        {
            "graphics engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , "no engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , VK_MAKE_VERSION(1, 0, 0)
        };

        const auto required_extensions = get_required_extensions
        <
            typename OPTIONS::debug::debug_callback
          , typename OPTIONS::window
        >();
        const auto required_layers = get_required_layers<typename OPTIONS::debug::validation_layers>();

        all_required_are_available(required_extensions, impl::get_available_instance_extensions());
        all_required_are_available(required_layers, impl::get_available_instance_layers());

        const vk::InstanceCreateInfo create_info
        {
            vk::InstanceCreateFlags()
          , &application_info
          , static_cast<uint32_t>(std::size(required_layers))
          , std::data(required_layers)
          , static_cast<uint32_t>(std::size(required_extensions))
          , std::data(required_extensions)
        };

        return vk::createInstanceUnique(create_info);
    }

} // namespace ge::impl::factory::instance
