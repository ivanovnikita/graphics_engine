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

        template<bool debug_callback_enabled, WindowOptions window_options>
        constexpr size_t get_required_extensions_count()
        {
            size_t count = 0;
            if constexpr (debug_callback_enabled)
            {
                ++count;
            }
            if constexpr (window_options != WindowOptions::NONE)
            {
                count += 2u;
            }
            return count;
        }

        template<bool debug_callback_enabled, WindowOptions window_options>
        constexpr auto get_required_extensions()
        {
            constexpr size_t size = get_required_extensions_count<debug_callback_enabled, window_options>();

            std::array<const char*, size> extensions{};

            size_t i = 0;
            if (debug_callback_enabled)
            {
                extensions[i] = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
                ++i;
            }

            if (window_options != WindowOptions::NONE)
            {
                extensions[i] = VK_KHR_SURFACE_EXTENSION_NAME;
                ++i;

                switch (window_options)
                {
                case WindowOptions::XCB:
                {
                    extensions[i] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
                    ++i;
                } break;
                case WindowOptions::WIN32:
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

        template<bool validation_layers_enabled>
        constexpr size_t get_required_layers_count()
        {
            size_t count = 0;
            if constexpr (validation_layers_enabled)
            {
                ++count;
            }
            return count;
        }

        template<bool validation_layers_enabled>
        constexpr auto get_required_layers()
        {
            constexpr size_t size = get_required_layers_count<validation_layers_enabled>();

            std::array<const char*, size> layers{};

            size_t i = 0;
            if (validation_layers_enabled)
            {
                layers[i] = "VK_LAYER_LUNARG_standard_validation";
                ++i;
            }
            return layers;
        }

    } // namespace impl

    template
    <
        bool debug_callback_enabled
      , bool validation_layers_enabled
      , WindowOptions window_options
    >
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

        const auto required_extensions = get_required_extensions<debug_callback_enabled, window_options>();
        const auto required_layers = get_required_layers<validation_layers_enabled>();

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
