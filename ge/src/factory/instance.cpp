#include "factory/instance.h"
#include "factory/tools.hpp"

#include <vector>

namespace ge::impl::factory::instance
{

    namespace
    {

        std::vector<std::string> get_available_instance_extensions()
        {
            return tools::extensions_names(vk::enumerateInstanceExtensionProperties());
        }

        std::vector<std::string> get_available_instance_layers()
        {
            return tools::layers_names(vk::enumerateInstanceLayerProperties());
        }

        std::vector<const char*> get_required_extensions
        (
            const OptionDebugCallback& option_debug_callback
          , const OptionWindow& option_window
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
                case WindowType::XCB:
                {
                    extensions.emplace_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
                } break;
                case WindowType::WIN32:
                {
                    // TODO: assert if not defined
#if defined(VK_KHR_WIN32_SURFACE_EXTENSION_NAME)
                    extensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
                } break;
                }
            }

            return extensions;
        }

        std::vector<const char*> get_required_layers(const OptionValidationLayers& option_validation_layers)
        {
            std::vector<const char*> layers;

            if (option_validation_layers.enabled)
            {
                layers.emplace_back("VK_LAYER_LUNARG_standard_validation");
            }

            return layers;
        }

    } // unnamed namespace

    vk::UniqueInstance create(const OptionsInstance& options)
    {
        using namespace tools;

        const vk::ApplicationInfo application_info
        {
            "graphics engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , "no engine"
          , VK_MAKE_VERSION(1, 0, 0)
          , VK_MAKE_VERSION(1, 0, 0)
        };

        const auto required_extensions = get_required_extensions(options.debug.debug_callback, options.window);
        const auto required_layers = get_required_layers(options.debug.validation_layers);

        all_required_are_available(required_extensions, get_available_instance_extensions());
        all_required_are_available(required_layers, get_available_instance_layers());

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

}
